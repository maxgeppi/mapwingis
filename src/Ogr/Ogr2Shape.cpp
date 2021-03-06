#include "stdafx.h"
#include "Ogr2Shape.h"
#include "OgrLabels.h"
#include "OgrConverter.h"
#include "GeoProjection.h"
#include "ShapefileHelper.h"

// *************************************************************
//		Layer2Shapefile()
// *************************************************************
IShapefile* Ogr2Shape::Layer2Shapefile(OGRLayer* layer, int maxFeatureCount, bool& isTrimmed, 
	OgrDynamicLoader* loader, ICallback* callback /*= NULL*/)
{
	if (!layer)	return NULL;

	IShapefile* sf = CreateShapefile(layer);

	if (sf) 
	{
		FillShapefile(layer, sf, maxFeatureCount, m_globalSettings.saveOgrLabels, callback, isTrimmed);
		
		// let's generate labels to unify API with dynamic mode
		if (loader->LabelExpression.GetLength() != 0)
		{
			CComPtr<ILabels> labels = NULL;
			sf->get_Labels(&labels);
			if (labels) 
			{
				labels->put_LineOrientation(loader->LabelOrientation);
				long count;
				ShpfileType type;
				sf->get_ShapefileType(&type);
				CComBSTR bstr(loader->LabelExpression);
				labels->Generate(bstr, loader->GetLabelPosition(type), VARIANT_TRUE, &count);
			}
		}
	}
	return sf;
}

// *************************************************************
//		CreateShapefile()
// *************************************************************
IShapefile* Ogr2Shape::CreateShapefile(OGRLayer* layer)
{
	layer->ResetReading();

	IShapefile* sf;
	VARIANT_BOOL vbretval;

	OGRFeature *poFeature;

	ShpfileType shpType = OgrConverter::GeometryType2ShapeType(layer->GetGeomType());

	// in case of queries or generic (untyped) geometry columns, type isn't defined
	// as quick fix let's fetch it from the first shape
	if (shpType == SHP_NULLSHAPE)
	{
		OGRFeatureDefn* defn = layer->GetLayerDefn();
		while ((poFeature = layer->GetNextFeature()) != NULL)
		{
			// TODO: perhaps loop through all features and find out the most frequent type
			OGRGeometry* geom = poFeature->GetGeometryRef();
			if (geom)
			{
				shpType = OgrConverter::GeometryType2ShapeType(geom->getGeometryType());
			}
			OGRFeature::DestroyFeature(poFeature);
			break;
		}
		layer->ResetReading();
	}

	ComHelper::CreateInstance(idShapefile, (IDispatch**)&sf);
	CComBSTR bstr = L"";
	sf->CreateNew(bstr, shpType, &vbretval);

	// setting projection for shapefile
	OGRSpatialReference* sr = layer->GetSpatialRef();
	if (sr)
	{
		// only if SRID is specified for geometry field
		IGeoProjection* gp = NULL;
		sf->get_GeoProjection(&gp);
		if (gp)
		{
			((CGeoProjection*)gp)->InjectSpatialReference(sr);
			gp->Release();
		}
	}

	/* ----------------------------------------------------------------- */
	/*		Converting of fields										 */
	/* ----------------------------------------------------------------- */
	IField * fld = NULL;

	// creating FID field to be able to write back to database
	bool hasFID = false;
	long fieldIndex = 0;
	CStringA name = layer->GetFIDColumn();
	if (name.GetLength() > 0)
	{
		ComHelper::CreateInstance(idField, (IDispatch**)&fld);
		fld->put_Type(INTEGER_FIELD);
		CComBSTR bstrName(name);
		fld->put_Name(bstrName);
		sf->EditInsertField(fld, &fieldIndex, NULL, &vbretval);
		fld->Release();
		hasFID = true;
	}

	OGRFeatureDefn *poFields = layer->GetLayerDefn();

	for (long iFld = 0; iFld < poFields->GetFieldCount(); iFld++)
	{
		ComHelper::CreateInstance(idField, (IDispatch**)&fld);

		OGRFieldDefn* oField = poFields->GetFieldDefn(iFld);
		OGRFieldType type = oField->GetType();

		if (type == OFTInteger)	fld->put_Type(INTEGER_FIELD);
		else if (type == OFTReal)	fld->put_Type(DOUBLE_FIELD);
		else if (type == OFTString)	fld->put_Type(STRING_FIELD);

		CComBSTR bstrName(oField->GetNameRef());
		fld->put_Name(bstrName);
		fld->put_Width((long)oField->GetWidth());
		fld->put_Precision((long)oField->GetPrecision());

		fieldIndex++;
		sf->EditInsertField(fld, &fieldIndex, NULL, &vbretval);
		fld->Release();
	}

	long numFields = ShapefileHelper::GetNumFields(sf);

	if (numFields == 0)
		ShapefileHelper::InsertMwShapeIdField(sf);

	return sf;
}

// *************************************************************
//		FillShapefile()
// *************************************************************
bool Ogr2Shape::FillShapefile(OGRLayer* layer, IShapefile* sf, int maxFeatureCount, bool loadLabels, ICallback* callback, bool& isTrimmed)
{
	if (!sf || !layer) return false;

	layer->ResetReading();

	int numFeatures = static_cast<int>(layer->GetFeatureCount());

	int count = 0;
	long percent = 0;
	USES_CONVERSION;
	CComBSTR key = L"";

	OGRFeature *poFeature;
	VARIANT_BOOL vbretval;

	CStringA name = layer->GetFIDColumn();
	bool hasFID = name.GetLength() > 0;
	ShpfileType shpType;
	sf->get_ShapefileType(&shpType);

	map<long, long> fids;

	OGRFeatureDefn *poFields = layer->GetLayerDefn();

	CComPtr<ILabels> labels = NULL;
	sf->get_Labels(&labels);
	OgrLabelsHelper::LabelFields labelFields;
	if (loadLabels) {
		if (!OgrLabelsHelper::GetLabelFields(layer, labelFields))
			loadLabels = false;
	}

	while ((poFeature = layer->GetNextFeature()) != NULL)
	{
		CallbackHelper::Progress(callback, count, numFeatures, "Converting geometries...", key.m_str, percent);
		count++;

		if (count > maxFeatureCount) {
			OGRFeature::DestroyFeature(poFeature);
			isTrimmed = true;
			break;
		}

		OGRGeometry *oGeom = poFeature->GetGeometryRef();

		IShape* shp = NULL;
		if (oGeom)
		{
			shp = OgrConverter::GeometryToShape(oGeom, Utility::ShapeTypeIsM(shpType));
		}

		if (!shp)
		{
			// insert null shape so that client can still access it
			ComHelper::CreateShape(&shp);
		}

		long numShapes;
		sf->get_NumShapes(&numShapes);
		sf->EditInsertShape(shp, &numShapes, &vbretval);
		shp->Release();

		if (hasFID)
		{
			CComVariant var;
			var.vt = VT_I4;
			var.lVal = static_cast<long>(poFeature->GetFID());
			sf->EditCellValue(0, numShapes, var, &vbretval);
		}

		double x = 0.0, y = 0.0, rotation = 0;
		CStringW text;

		for (int iFld = 0; iFld < poFields->GetFieldCount(); iFld++)
		{
			OGRFieldDefn* oField = poFields->GetFieldDefn(iFld);
			OGRFieldType type = oField->GetType();

			CComVariant var;
			if (type == OFTInteger)
			{
				var.vt = VT_I4;
				var.lVal = poFeature->GetFieldAsInteger(iFld);
			}
			else if (type == OFTReal)
			{
				var.vt = VT_R8;
				var.dblVal = poFeature->GetFieldAsDouble(iFld);
			}
			else //if (type == OFTString )
			{
				var.vt = VT_BSTR;
				var.bstrVal = A2BSTR(poFeature->GetFieldAsString(iFld));		// BSTR will be cleared by CComVariant destructor
			}
			sf->EditCellValue(hasFID ? iFld + 1 : iFld, numShapes, var, &vbretval);

			if (loadLabels)
			{
				if (iFld == labelFields.X) x = var.dblVal;
				if (iFld == labelFields.Y) y = var.dblVal;
				if (iFld == labelFields.Text) text = OgrHelper::OgrString2Unicode(poFeature->GetFieldAsString(iFld));
				if (iFld == labelFields.Rotation) rotation = var.dblVal;
			}
		}

		if (loadLabels) {
			CComBSTR bstr(text);
			labels->AddLabel(bstr, x, y, rotation);
		}

		OGRFeature::DestroyFeature(poFeature);
	}
	CallbackHelper::ProgressCompleted(callback);

	sf->RefreshExtents(&vbretval);
	Utility::ClearShapefileModifiedFlag(sf);		// inserted shapes were marked as modified, correct this
	return true;
}