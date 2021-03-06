// This file stores properties of shapefile layers and separate shapes
// The properties are obsolete, new ShapeDrawingOptions and ShapeCategory classes should be used instead
// Where possible the call where redirected to the new classes
#pragma once
#include "stdafx.h"
#include "Map.h"
#include "ShapeDrawingOptions.h"
#include "Shapefile.h"
#include "Labels.h"
#include "SelectionHelper.h"
#include "ShapeStyleHelper.h"
#include "ShapefileHelper.h"
#include "Structures.h"
#include "EditorHelper.h"

// TODO: the following properties for the new symbology must be implemented
// ShapeLayerLineStipple
// ShapeLayerFillStipple
// ShapeLayerPointType
// SetUDPointType

#pragma region General
// *************************************************************
//	  ShapefileDrawingOptions
// *************************************************************
// Returns drawing options for a given shapefile
CDrawingOptionsEx* CMapView::get_ShapefileDrawingOptions(long layerHandle)
{
	if (layerHandle >= 0 && layerHandle < (long)_allLayers.size())
	{
		Layer * layer = _allLayers[layerHandle];
		if( layer->IsShapefile() )
		{
			IShapefile* sf = NULL;
			if (layer->QueryShapefile(&sf))
			{
				IShapeDrawingOptions* options = NULL;
				sf->get_DefaultDrawingOptions(&options);
				sf->Release();
				if (options)
				{
					CDrawingOptionsEx* retVal = ((CShapeDrawingOptions*)options)->get_UnderlyingOptions();
					options->Release();
					return retVal;
				}
			}
		}
		else
		{
			this->ErrorMessage(tkUNEXPECTED_LAYER_TYPE);
		}
	}
	else
	{
		this->ErrorMessage(tkINVALID_LAYER_HANDLE);
	}
	return NULL;
}

// *************************************************************
//	  GetShapefileType
// *************************************************************
// Returns type of the shapefile with a given handle
ShpfileType CMapView::get_ShapefileType(long layerHandle)
{
	if (layerHandle >= 0 && layerHandle < (long)_allLayers.size())
	{
		Layer * layer = _allLayers[layerHandle];
		if( layer->IsShapefile() )
		{
			IShapefile* sf = NULL;
			layer->QueryShapefile(&sf);
			if (sf)
			{
				ShpfileType type;
				sf->get_ShapefileType(&type);
				sf->Release();
				return type;
			}
		}
		else
		{
			this->ErrorMessage(tkUNEXPECTED_LAYER_TYPE);
		}
	}
	else
	{
		this->ErrorMessage(tkINVALID_LAYER_HANDLE);
	}
	return SHP_NULLSHAPE;
}

// ***************************************************************
//		get_ShapefileLayer()
// ***************************************************************
// Returns pointer to the shapefile layer with the given index, checks it's type
Layer* CMapView::GetShapefileLayer(long layerHandle)
{
	if (layerHandle >= 0 && layerHandle < (long)_allLayers.size())
	{
		Layer * layer = _allLayers[layerHandle];
		if( layer->IsShapefile() )
		{
			return layer;
		}
		else
		{
			this->ErrorMessage(tkUNEXPECTED_LAYER_TYPE);
			return NULL;
		}
	}
	else
	{
		this->ErrorMessage(tkINVALID_LAYER_HANDLE);
		return NULL;
	}
}

// ***************************************************************
//		get_Layer()
// ***************************************************************
Layer* CMapView::GetLayer(long layerHandle)
{
	if (layerHandle >= 0 && layerHandle < (long)_allLayers.size())
	{
		return _allLayers[layerHandle];
	}
	else
	{
		this->ErrorMessage(tkINVALID_LAYER_HANDLE);
		return NULL;
	}
}

// ***************************************************************
//		IsValidLayer()
// ***************************************************************
bool CMapView::IsValidLayer( long layerHandle )
{
	if (layerHandle >= 0 && layerHandle < (long)_allLayers.size())
	{
		return (_allLayers[layerHandle]!=NULL)?true:false;
	}
	else
	{
		return false;
	}
}

// ***************************************************************
//		IsValidShape()
// ***************************************************************
bool CMapView::IsValidShape( long layerHandle, long shape )
{	
	if( IsValidLayer(layerHandle) )
	{
		Layer * l = _allLayers[layerHandle];
		if( l->IsShapefile() )
		{	
			this->AlignShapeLayerAndShapes(l);
			
			IShapefile * ishp = NULL;
			l->QueryShapefile(&ishp);

			long numShapes = 0;
			if( ishp == NULL )
			{
				numShapes = 0;
			}
			else
			{	
				ishp->get_NumShapes(&numShapes);
				ishp->Release();
				ishp = NULL;
			}
			
			if( shape >= 0 && shape < (long)numShapes )
			{
				return TRUE;
			}
			else
			{	
				ErrorMessage(tkINDEX_OUT_OF_BOUNDS);
				return FALSE;
			}
		}
		else
		{	
			ErrorMessage(tkUNEXPECTED_LAYER_TYPE);
			return FALSE;
		}
	}
	else
	{
		ErrorMessage(tkINVALID_LAYER_HANDLE);
		return FALSE;
	}
	
}
#pragma endregion

#pragma region LayerProperties

#pragma region Points

// *****************************************************************
//		GetShapeLayerDrawPoint()
// *****************************************************************
BOOL CMapView::GetShapeLayerDrawPoint(long LayerHandle)
{
	CDrawingOptionsEx* options = get_ShapefileDrawingOptions(LayerHandle);
	if (options)
	{
		ShpfileType type = this->get_ShapefileType(LayerHandle);
		if (type == SHP_POINT || type == SHP_POINTM || type == SHP_POINTZ ||
			type == SHP_MULTIPOINT || type == SHP_MULTIPOINTZ || type == SHP_MULTIPOINTM)
		{
			return options->fillVisible?TRUE:FALSE;
		}
		else
		{
			return options->verticesVisible?TRUE:FALSE;
		}
	}
	else
	{
		return FALSE;
	}
}

// *****************************************************************
//		SetShapeLayerDrawPoint()
// *****************************************************************
void CMapView::SetShapeLayerDrawPoint(long LayerHandle, BOOL bNewValue)
{
	CDrawingOptionsEx* options = get_ShapefileDrawingOptions(LayerHandle);
	
	if (options)
	{
		ShpfileType type = this->get_ShapefileType(LayerHandle);
		if (type == SHP_POINT || type == SHP_POINTM || type == SHP_POINTZ ||
			type == SHP_MULTIPOINT || type == SHP_MULTIPOINTZ || type == SHP_MULTIPOINTM)
		{
			options->fillVisible = (bNewValue == TRUE);
			options->linesVisible = (bNewValue == TRUE);

		}
		else
		{
			options->verticesVisible = bNewValue;
		}
		ScheduleLayerRedraw();
	}
}

// *****************************************************************
//		GetShapeLayerPointSize()
// *****************************************************************
float CMapView::GetShapeLayerPointSize(long LayerHandle)
{
	CDrawingOptionsEx* options = get_ShapefileDrawingOptions(LayerHandle);
	if (options)
		return options->pointSize;
	else
		return 0.0f;
}

// *****************************************************************
//		SetShapeLayerPointSize()
// *****************************************************************
void CMapView::SetShapeLayerPointSize(long LayerHandle, float newValue)
{
	if( newValue < 0.0 )
		newValue = 0.0;

	CDrawingOptionsEx* options = get_ShapefileDrawingOptions(LayerHandle);
	if (options)
	{		
		options->pointSize = newValue;
		ScheduleLayerRedraw();
	}
}

// *****************************************************************
//		ShapeLayerPointType()
// *****************************************************************
short CMapView::GetShapeLayerPointType(long LayerHandle)
{
	CDrawingOptionsEx* options = get_ShapefileDrawingOptions(LayerHandle);
	if (options)
	{
		ErrorMessage(tkPROPERTY_NOT_IMPLEMENTED);	// TODO: write conversions between point types
		return ptSquare;
	}
	else
		return 0;
}

// *****************************************************************
//		ShapeLayerPointType()
// *****************************************************************
void CMapView::SetShapeLayerPointType(long LayerHandle, short nNewValue)
{
	CDrawingOptionsEx* options = get_ShapefileDrawingOptions(LayerHandle);
	if (options)
	{
		ErrorMessage(tkPROPERTY_NOT_IMPLEMENTED);	// TODO: write conversions between point types	
	}
}	

// *****************************************************************
//		GetShapeLayerPointColor()
// *****************************************************************
OLE_COLOR CMapView::GetShapeLayerPointColor(long LayerHandle)
{
	CDrawingOptionsEx* options = get_ShapefileDrawingOptions(LayerHandle);
	if (options)
	{
		ShpfileType type = this->get_ShapefileType(LayerHandle);
		if (type == SHP_POINT || type == SHP_POINTM || type == SHP_POINTZ ||
			type == SHP_MULTIPOINT || type == SHP_MULTIPOINTZ || type == SHP_MULTIPOINTM)
		{
			return options->fillColor;
		}
		else
		{
			return options->verticesColor;
		}
	}
	else
	{
		return RGB(0,0,0);
	}
}

// *****************************************************************
//		ShapeLayerPointColor()
// *****************************************************************
void CMapView::SetShapeLayerPointColor(long LayerHandle, OLE_COLOR nNewValue)
{
	CDrawingOptionsEx* options = get_ShapefileDrawingOptions(LayerHandle);
	if (options)
	{
		ShpfileType type = this->get_ShapefileType(LayerHandle);
		if (type == SHP_POINT || type == SHP_POINTM || type == SHP_POINTZ ||
			type == SHP_MULTIPOINT || type == SHP_MULTIPOINTZ || type == SHP_MULTIPOINTM)
		{
			options->fillColor = nNewValue;
		}
		else
		{
			options->verticesColor = nNewValue;
		}
		ScheduleLayerRedraw();
	}
}

// *****************************************************************
//		GetUDPointType()
// *****************************************************************
LPDISPATCH CMapView::GetUDPointType(long LayerHandle)
{
	CDrawingOptionsEx* options = get_ShapefileDrawingOptions(LayerHandle);
	if (options)
	{
		if (options->picture)
		{
			options->picture->AddRef();
			return options->picture;
		}
		else
			return NULL;
	}
	else
		return NULL;
}

// *****************************************************************
//		SetUDPointType()
// *****************************************************************
void CMapView::SetUDPointType(long LayerHandle, LPDISPATCH newValue)
{
	CDrawingOptionsEx* options = get_ShapefileDrawingOptions(LayerHandle);
	if (options)
	{
		IImage * iimg = NULL;
		newValue->QueryInterface( IID_IImage, (void**)&iimg );
		ComHelper::SetRef((IDispatch*)iimg, (IDispatch**)&options->picture, true);
		if (iimg)
		{
			iimg->Release();
		}
	}
}
#pragma endregion

#pragma region Lines
// *****************************************************************
//		GetShapeLayerDrawLine()
// *****************************************************************
BOOL CMapView::GetShapeLayerDrawLine(long LayerHandle)
{
	CDrawingOptionsEx* options = get_ShapefileDrawingOptions(LayerHandle);
	if (options)
		return (BOOL)options->linesVisible;
	else
		return FALSE;
}

// *****************************************************************
//		SetShapeLayerDrawLine()
// *****************************************************************
void CMapView::SetShapeLayerDrawLine(long LayerHandle, BOOL bNewValue)
{
	CDrawingOptionsEx* options = get_ShapefileDrawingOptions(LayerHandle);
	if (options)
	{
		options->linesVisible = (bNewValue == TRUE);
		ScheduleLayerRedraw();
	}
}	

// ***********************************************************
//		GetShapeLayerLineColor()
// ***********************************************************
OLE_COLOR CMapView::GetShapeLayerLineColor(long LayerHandle)
{
	CDrawingOptionsEx* options = get_ShapefileDrawingOptions(LayerHandle);
	if (options)
		return options->lineColor;
	else
		return RGB(0,0,0);
}

// *****************************************************************
//		SetShapeLayerLineColor()
// *****************************************************************
void CMapView::SetShapeLayerLineColor(long LayerHandle, OLE_COLOR nNewValue)
{
	CDrawingOptionsEx* options = get_ShapefileDrawingOptions(LayerHandle);
	if (options)
	{
		options->lineColor = nNewValue;
		ScheduleLayerRedraw();
	}
}

// *****************************************************************
//		GetShapeLayerLineWidth()
// *****************************************************************
float CMapView::GetShapeLayerLineWidth(long LayerHandle)
{
	CDrawingOptionsEx* options = get_ShapefileDrawingOptions(LayerHandle);
	if (options)
	{
		return options->lineWidth;
	}
	else
	{
		return 0.0f;
	}
}

// *****************************************************************
//		ShapeLayerLineWidth()
// *****************************************************************
void CMapView::SetShapeLayerLineWidth(long LayerHandle, float newValue)
{
	if( newValue < 0.0 )
			newValue = 0.0;

	CDrawingOptionsEx* options = get_ShapefileDrawingOptions(LayerHandle);
	if (options)
	{
		options->lineWidth = newValue;
		ScheduleLayerRedraw();
	}
}

// *****************************************************************
//		ShapeLayerLineStipple()
// *****************************************************************
short CMapView::GetShapeLayerLineStipple(long LayerHandle)
{
	ErrorMessage(tkPROPERTY_NOT_IMPLEMENTED);
	CDrawingOptionsEx* options = get_ShapefileDrawingOptions(LayerHandle);
	if (options)
		return (short)options->lineStipple;				// TODO: convert between enumerations
	else
		return -1;
}

// *****************************************************************
//		SetShapeLayerLineStipple()
// *****************************************************************
void CMapView::SetShapeLayerLineStipple(long LayerHandle, short nNewValue)
{
	ErrorMessage(tkPROPERTY_NOT_IMPLEMENTED);
	CDrawingOptionsEx* options = get_ShapefileDrawingOptions(LayerHandle);
	if (options)
	{	
		options->lineStipple = (tkDashStyle)nNewValue;	// TODO: convert between enumerations
		ScheduleLayerRedraw();
	}		
}

// *****************************************************************
//		GetUDLineStipple()
// *****************************************************************
long CMapView::GetUDLineStipple(long LayerHandle)
{
	ErrorMessage(tkPROPERTY_NOT_IMPLEMENTED);
	return 0;
}

// *****************************************************************
//		UDLineStipple()
// *****************************************************************
void CMapView::SetUDLineStipple(long LayerHandle, long nNewValue)
{
	ErrorMessage(tkPROPERTY_DEPRECATED);
}
#pragma endregion

#pragma region Fill

// *****************************************************************
//		GetShapeLayerDrawFill()
// *****************************************************************
BOOL CMapView::GetShapeLayerDrawFill(long LayerHandle)
{
	CDrawingOptionsEx* options = get_ShapefileDrawingOptions(LayerHandle);
	if (options)
		return (BOOL)options->fillVisible;
	else
		return FALSE;
}

// *****************************************************************
//		SetShapeLayerDrawFill()
// *****************************************************************
void CMapView::SetShapeLayerDrawFill(long LayerHandle, BOOL bNewValue)
{
	CDrawingOptionsEx* options = get_ShapefileDrawingOptions(LayerHandle);
	if (options)
	{
		options->fillVisible = (bNewValue == TRUE);
		ScheduleLayerRedraw();
	}
}

// *********************************************************
//		GetShapeLayerFillColor()
// *********************************************************
OLE_COLOR CMapView::GetShapeLayerFillColor(long LayerHandle)
{
	CDrawingOptionsEx* options = get_ShapefileDrawingOptions(LayerHandle);
	if (options)
		return options->fillColor;
	else
		return RGB(0,0,0);
}

// **********************************************************
//		SetShapeLayerFillColor()
// **********************************************************
void CMapView::SetShapeLayerFillColor(long LayerHandle, OLE_COLOR nNewValue)
{
	CDrawingOptionsEx* options = get_ShapefileDrawingOptions(LayerHandle);
	if (options)
	{
		options->fillColor = nNewValue;
		ScheduleLayerRedraw();
	}
}

// *****************************************************************
//		GetShapeLayerFillTransparency
// *****************************************************************
float CMapView::GetShapeLayerFillTransparency(long LayerHandle)
{
	CDrawingOptionsEx* options = get_ShapefileDrawingOptions(LayerHandle);
	if (options)
	{
		return options->fillTransparency/255.0f;
	}
	else
	{
		return 0.0f;
	}
}

// *****************************************************************
//		SetShapeLayerFillTransparency
// *****************************************************************
void CMapView::SetShapeLayerFillTransparency(long LayerHandle, float newValue)
{
	if( newValue < 0.0 )		newValue = 0.0;
	else if( newValue > 1.0 )	newValue = 1.0;

	CDrawingOptionsEx* options = get_ShapefileDrawingOptions(LayerHandle);
	if (options)
	{
		options->fillTransparency = newValue * 255.0f;
		ScheduleLayerRedraw();
	}
}
#pragma endregion

#pragma region FillStipple
// *****************************************************************
//		GetShapeLayerFillStipple()
// *****************************************************************
short CMapView::GetShapeLayerFillStipple(long LayerHandle)
{
	ErrorMessage(tkPROPERTY_NOT_IMPLEMENTED);
	CDrawingOptionsEx* options = get_ShapefileDrawingOptions(LayerHandle);
	if (options)
	{
		return fsNone;			// TODO: write conversion between tkFillStipple and tkGDIPlusHatchStyle enumerations
	}
	else
	{
		return -1;
	}
}

// *****************************************************************
//		SetShapeLayerFillStipple()
// *****************************************************************
void CMapView::SetShapeLayerFillStipple(long LayerHandle, short nNewValue)
{
	ErrorMessage(tkPROPERTY_NOT_IMPLEMENTED);
	CDrawingOptionsEx* options = get_ShapefileDrawingOptions(LayerHandle);
	if (options)	
	{
		// TODO: write conversion between tkFillStipple and tkGDIPlusHatchStyle enumerations
		ScheduleLayerRedraw();
	}
}	

// *****************************************************************
//		GetShapeLayerStippleTransparent()
// *****************************************************************
BOOL CMapView::GetShapeLayerStippleTransparent(long LayerHandle)
{
	CDrawingOptionsEx* options = get_ShapefileDrawingOptions(LayerHandle);
	if (options)
		return (BOOL)options->fillBgTransparent;
	else
		return TRUE;
}

// *****************************************************************
//		SetShapeLayerStippleTransparent()
// *****************************************************************
void CMapView::SetShapeLayerStippleTransparent(long LayerHandle, BOOL nNewValue)
{
	CDrawingOptionsEx* options = get_ShapefileDrawingOptions(LayerHandle);
	if (options)
	{
		options->fillBgTransparent = (nNewValue == TRUE ? true : false);
		ScheduleLayerRedraw();
	}
}

// *****************************************************************
//		GetShapeLayerStippleColor()
// *****************************************************************
OLE_COLOR CMapView::GetShapeLayerStippleColor(long LayerHandle)
{
	CDrawingOptionsEx* options = get_ShapefileDrawingOptions(LayerHandle);
	if (options)
		return options->fillBgColor;
	else
		return RGB(0,0,0);
}

// *****************************************************************
//		SetShapeLayerStippleColor()
// *****************************************************************
void CMapView::SetShapeLayerStippleColor(long LayerHandle, OLE_COLOR nNewValue)
{
	CDrawingOptionsEx* options = get_ShapefileDrawingOptions(LayerHandle);
	if (options)
	{
		options->fillBgColor = nNewValue;
		ScheduleLayerRedraw();
	}
}

// *****************************************************************
//		GetUDFillStipple()
// *****************************************************************
long CMapView::GetUDFillStipple(long LayerHandle, long StippleRow)
{
	ErrorMessage(tkPROPERTY_DEPRECATED);
	return 0;
}

// *****************************************************************
//		SetUDFillStipple()
// *****************************************************************
void CMapView::SetUDFillStipple(long LayerHandle, long StippleRow, long nNewValue)
{
		ErrorMessage(tkPROPERTY_DEPRECATED);
}
#pragma endregion

#pragma endregion
	
#pragma region REGION PerShapeDrawingOptions

#pragma region ShapeVisible
// *****************************************************************
//		GetShapeVisible()
// *****************************************************************
BOOL CMapView::GetShapeVisible(long LayerHandle, long Shape)
{
	ErrorMessage(tkPROPERTY_DEPRECATED);
	return FALSE;
}

// *****************************************************************
//		SetShapeVisible()
// *****************************************************************
void CMapView::SetShapeVisible(long LayerHandle, long Shape, BOOL bNewValue)
{
	ErrorMessage(tkPROPERTY_DEPRECATED);
}
#pragma endregion

#pragma region Points

// *****************************************************************
//		GetShapeDrawPoint()
// *****************************************************************
BOOL CMapView::GetShapeDrawPoint(long LayerHandle, long Shape)
{
	ErrorMessage(tkPROPERTY_DEPRECATED);
	return FALSE;
}

// *****************************************************************
//		SetShapeDrawPoint()
// *****************************************************************
void CMapView::SetShapeDrawPoint(long LayerHandle, long Shape, BOOL bNewValue)
{
	ErrorMessage(tkPROPERTY_DEPRECATED);
}

// *****************************************************************
//		GetShapePointColor()
// *****************************************************************
OLE_COLOR CMapView::GetShapePointColor(long LayerHandle, long Shape)
{
	ErrorMessage(tkPROPERTY_DEPRECATED);
	return RGB(220,220,220);
}

// *****************************************************************
//		SetShapePointColor()
// *****************************************************************
void CMapView::SetShapePointColor(long LayerHandle, long Shape, OLE_COLOR nNewValue)
{
	ErrorMessage(tkPROPERTY_DEPRECATED);
}

// *****************************************************************
//		GetShapePointSize()
// *****************************************************************
float CMapView::GetShapePointSize(long LayerHandle, long Shape)
{
	ErrorMessage(tkPROPERTY_DEPRECATED);
	return 0.0f;
}

// *****************************************************************
//		SetShapePointSize()
// *****************************************************************
void CMapView::SetShapePointSize(long LayerHandle, long Shape, float newValue)
{
	ErrorMessage(tkPROPERTY_DEPRECATED);
}

// *****************************************************************
//		GetShapePointType()
// *****************************************************************
short CMapView::GetShapePointType(long LayerHandle, long Shape)
{
	ErrorMessage(tkPROPERTY_DEPRECATED);
	return 0;
}

// *****************************************************************
//		SetShapePointType()
// *****************************************************************
void CMapView::SetShapePointType(long LayerHandle, long Shape, short nNewValue)
{
	ErrorMessage(tkPROPERTY_DEPRECATED);
}
#pragma endregion

#pragma region Lines

// *****************************************************************
//		GetShapeDrawLine()
// *****************************************************************
BOOL CMapView::GetShapeDrawLine(long LayerHandle, long Shape)
{
	ErrorMessage(tkPROPERTY_DEPRECATED);
	return FALSE;
}

// *****************************************************************
//		SetShapeDrawLine()
// *****************************************************************
void CMapView::SetShapeDrawLine(long LayerHandle, long Shape, BOOL bNewValue)
{
	ErrorMessage(tkPROPERTY_DEPRECATED);
}

// *****************************************************************
//		GetShapeLineColor()
// *****************************************************************
OLE_COLOR CMapView::GetShapeLineColor(long LayerHandle, long Shape)
{
	ErrorMessage(tkPROPERTY_DEPRECATED);
	return RGB(220,220,220);
}

// *****************************************************************
//		SetShapeLineColor()
// *****************************************************************
void CMapView::SetShapeLineColor(long LayerHandle, long Shape, OLE_COLOR nNewValue)
{
	ErrorMessage(tkPROPERTY_DEPRECATED);
}

// *****************************************************************
//		GetShapeLineWidth()
// *****************************************************************
float CMapView::GetShapeLineWidth(long LayerHandle, long Shape)
{
	ErrorMessage(tkPROPERTY_DEPRECATED);
	return 0.0f;
}

// *****************************************************************
//		SetShapeLineWidth()
// *****************************************************************
void CMapView::SetShapeLineWidth(long LayerHandle, long Shape, float newValue)
{
	ErrorMessage(tkPROPERTY_DEPRECATED);
}

// *****************************************************************
//		GetShapeLineStipple()
// *****************************************************************
short CMapView::GetShapeLineStipple(long LayerHandle, long Shape)
{
	ErrorMessage(tkPROPERTY_DEPRECATED);
	return -1;
}

// *****************************************************************
//		SetShapeLineStipple()
// *****************************************************************
void CMapView::SetShapeLineStipple(long LayerHandle, long Shape, short nNewValue)
{
	ErrorMessage(tkPROPERTY_DEPRECATED);
}
#pragma endregion

#pragma region Fill

// *****************************************************************
//		GetShapeDrawFill()
// *****************************************************************
BOOL CMapView::GetShapeDrawFill(long LayerHandle, long Shape)
{
	ErrorMessage(tkPROPERTY_DEPRECATED);
	return RGB(220,220,220);
}

// *****************************************************************
//		SetShapeDrawFill()
// *****************************************************************
void CMapView::SetShapeDrawFill(long LayerHandle, long Shape, BOOL bNewValue)
{
	ErrorMessage(tkPROPERTY_DEPRECATED);
}

// *****************************************************************
//		GetShapeFillColor()
// *****************************************************************
OLE_COLOR CMapView::GetShapeFillColor(long LayerHandle, long Shape)
{
	ErrorMessage(tkPROPERTY_DEPRECATED);
	return RGB(220,220,220);
}

// *****************************************************************
//		SetShapeFillColor()
// *****************************************************************
void CMapView::SetShapeFillColor(long LayerHandle, long Shape, OLE_COLOR nNewValue)
{
	ErrorMessage(tkPROPERTY_DEPRECATED);
}

// *****************************************************************
//		GetShapeFillTransparency()
// *****************************************************************
float CMapView::GetShapeFillTransparency(long LayerHandle, long Shape)
{
	ErrorMessage(tkPROPERTY_DEPRECATED);
	return 0.0f;
}

// *****************************************************************
//		SetShapeFillTransparency()
// *****************************************************************
void CMapView::SetShapeFillTransparency(long LayerHandle, long Shape, float newValue)
{
	ErrorMessage(tkPROPERTY_DEPRECATED);
}
#pragma endregion 

#pragma region FillStipple

// *****************************************************************
//		GetShapeFillStipple()
// *****************************************************************
short CMapView::GetShapeFillStipple(long LayerHandle, long Shape)
{
	ErrorMessage(tkPROPERTY_DEPRECATED);
	return -1;
}

// *****************************************************************
//		SetShapeFillStipple()
// *****************************************************************
void CMapView::SetShapeFillStipple(long LayerHandle, long Shape, short nNewValue)
{
	ErrorMessage(tkPROPERTY_DEPRECATED);
}

// *****************************************************************
//		GetShapeStippleTransparent()
// *****************************************************************
BOOL CMapView::GetShapeStippleTransparent(long LayerHandle, long Shape)
{
	ErrorMessage(tkPROPERTY_DEPRECATED);
	return TRUE;
}

// *****************************************************************
//		SetShapeStippleTransparent()
// *****************************************************************
void CMapView::SetShapeStippleTransparent(long LayerHandle, long Shape, BOOL nNewValue)
{
	ErrorMessage(tkPROPERTY_DEPRECATED);
}

// *****************************************************************
//		GetShapeStippleColor()
// *****************************************************************
OLE_COLOR CMapView::GetShapeStippleColor(long LayerHandle, long Shape)
{
	ErrorMessage(tkPROPERTY_DEPRECATED);
	return RGB(220,220,220);
}

// *****************************************************************
//		SetShapeStippleColor()
// *****************************************************************
void CMapView::SetShapeStippleColor(long LayerHandle, long Shape, OLE_COLOR nNewValue)
{
	ErrorMessage(tkPROPERTY_DEPRECATED);
}
#pragma endregion

#pragma endregion

#pragma region PointImageList
// *****************************************************************
//		UDPointImageListCount()
// *****************************************************************
long CMapView::get_UDPointImageListCount(long LayerHandle)
{
	ErrorMessage(tkPROPERTY_DEPRECATED);
	return 0;
}

// *****************************************************************
//		get_UDPointImageListItem()
// *****************************************************************
IDispatch* CMapView::get_UDPointImageListItem(long LayerHandle, long ImageIndex)
{
	ErrorMessage(tkPROPERTY_DEPRECATED);
	return NULL;
}

// *****************************************************************
//		ClearUDPointImageList()
// *****************************************************************
void CMapView::ClearUDPointImageList(long LayerHandle)
{
	ErrorMessage(tkPROPERTY_DEPRECATED);
}

// *****************************************************************
//		set_UDPointImageListAdd()
// *****************************************************************
long CMapView::set_UDPointImageListAdd(long LayerHandle, LPDISPATCH newValue)
{
	ErrorMessage(tkPROPERTY_DEPRECATED);
	return -1;
}

	// *****************************************************************
//		SetShapePointImageListID()
// *****************************************************************
void CMapView::SetShapePointImageListID(long LayerHandle, long Shape, long ImageIndex)
{
	ErrorMessage(tkPROPERTY_DEPRECATED);
}

// *****************************************************************
//		GetShapePointImageListID()
// *****************************************************************
long CMapView::GetShapePointImageListID(long LayerHandle, long Shape)
{
	ErrorMessage(tkPROPERTY_DEPRECATED);
	return -1;
}
#pragma endregion

#pragma region PointFontCharList
// *****************************************************************
//		set_UDPointFontCharListAdd()
// *****************************************************************
long CMapView::set_UDPointFontCharListAdd(long LayerHandle, long newValue, OLE_COLOR color)
{
	ErrorMessage(tkPROPERTY_DEPRECATED);
	return -1;
}

// *****************************************************************
//		set_UDPointFontCharFont()
// *****************************************************************
void CMapView::set_UDPointFontCharFont(long LayerHandle, LPCTSTR FontName, float FontSize, BOOL isBold, BOOL isItalic, BOOL isUnderline)
{
	ErrorMessage(tkPROPERTY_DEPRECATED);
}

// *****************************************************************
//		set_UDPointFontCharFontSize()
// *****************************************************************
void CMapView::set_UDPointFontCharFontSize(long LayerHandle,  float FontSize)
{
	ErrorMessage(tkPROPERTY_DEPRECATED);
}

// *****************************************************************
//		SetShapePointFontCharListID()
// *****************************************************************
void CMapView::SetShapePointFontCharListID(long LayerHandle, long Shape, long FontCharIndex)
{
	ErrorMessage(tkPROPERTY_DEPRECATED);
}

// *****************************************************************
//		GetShapePointFontCharListID()
// *****************************************************************
long CMapView::GetShapePointFontCharListID(long LayerHandle, long Shape)
{
	ErrorMessage(tkPROPERTY_DEPRECATED);
	return -1;
}
#pragma endregion

#pragma region Loop through shapefiles

// ************************************************************
//		GetProjectedTolerance
// ************************************************************
double CMapView::GetProjectedTolerance(double xScreen, double yScreen, double tolerance)
{
	double x, y, x2, y2;
	this->PixelToProjection(xScreen, yScreen, x, y);
	this->PixelToProjection(xScreen + tolerance, yScreen + tolerance, x2, y2);
	return sqrt(pow(x - x2, 2.0) + pow(y - y2, 2.0));
}

// ************************************************************
//		FindSnapPoint
// ************************************************************
VARIANT_BOOL CMapView::FindSnapPointCore(double xScreen, double yScreen, double* xFound, double* yFound)
{
	return FindSnapPoint(GetMouseTolerance(ToleranceSnap, false), xScreen, yScreen, xFound, yFound);
}
VARIANT_BOOL CMapView::FindSnapPoint(double tolerance, double xScreen, double yScreen, double* xFound, double* yFound)
{
	double maxDist = GetProjectedTolerance(xScreen, yScreen, tolerance);
	double x, y;
	this->PixelToProjection(xScreen, yScreen, x, y);

	long shapeIndex;
	long pointIndex;
	VARIANT_BOOL vb;
	double distance;
	
	double minDist = DBL_MAX;
	IShapefile* foundShapefile = NULL;
	long foundShapeIndex;
	long foundPointIndex;

	bool digitizing = EditorHelper::IsDigitizingCursor((tkCursorMode)m_cursorMode);
	tkLayerSelection behavior;
	_shapeEditor->get_SnapBehavior(&behavior);
	long currentHandle = -1;
	bool currentLayerOnly = behavior == lsActiveLayer && digitizing;
	if (currentLayerOnly)
		_shapeEditor->get_LayerHandle(&currentHandle);

	for(long i = 0; i < this->GetNumLayers(); i++)
	{
		long layerHandle = this->GetLayerHandle(i);
		if (currentLayerOnly && layerHandle != currentHandle) 
			continue;

		Layer* l = GetLayer(layerHandle);
		if (!l || !l->wasRendered) continue;

		CComPtr<IShapefile> sf = NULL;
		sf.Attach(this->GetShapefile(layerHandle));
		if (sf)
		{
			VARIANT_BOOL snappable;
			sf->get_Snappable(&snappable);
			if (snappable) 
			{
				sf->GetClosestVertex(x, y, maxDist, &shapeIndex, &pointIndex, &distance, &vb);
				if (vb)
				{
					if (distance < minDist)
					{
						minDist = distance;
						foundShapefile = sf;
						foundPointIndex = pointIndex;
						foundShapeIndex = shapeIndex;
					}
				}
			}
		}
	}

	bool result = false;
	if (minDist != DBL_MAX && foundShapefile)
	{
		IShape* shape = NULL;
		foundShapefile->get_Shape(foundShapeIndex, &shape);
		if (shape)
		{
			shape->get_XY(foundPointIndex, xFound, yFound, &vb);
			shape->Release();
			result = true;
		}
	}
	return result;
}

// ************************************************************
//		SelectLayerHandles
// ************************************************************
bool CMapView::SelectLayerHandles(LayerSelector selector, std::vector<int>& layers)
{
	IShapefile * sf = NULL;
	for (int i = 0; i < (int)_activeLayers.size(); i++)
	{
		int handle = GetLayerHandle(i);
		bool result = CheckLayer(selector, handle);
		if (result)
			layers.push_back(handle);
	}
	return layers.size() > 0;
}

// ************************************************************
//		LayerIsIdentifiable
// ************************************************************
VARIANT_BOOL CMapView::LayerIsIdentifiable(long layerHandle, IShapefile* sf)
{
	VARIANT_BOOL result = VARIANT_FALSE;
	tkIdentifierMode mode;
	_identifier->get_IdentifierMode(&mode);
	if (mode == imAllLayers) {
		sf->get_Identifiable(&result);
	}
	if (mode == imSingleLayer) {
		long activeHandle;
		_identifier->get_ActiveLayer(&activeHandle);
		if (layerHandle == activeHandle)
			result = VARIANT_TRUE;
	}
	return result;
}

// ************************************************************
//		CheckLayer
// ************************************************************
bool CMapView::CheckLayer(LayerSelector selector, int layerHandle)
{
	CComPtr<IShapefile> sf = NULL;
	Layer* layer = _allLayers[layerHandle];
	if (layer->IsShapefile())
	{
		if (layer->QueryShapefile(&sf))
		{
			if (!layer->wasRendered) return false;
			VARIANT_BOOL result = VARIANT_FALSE;
			switch (selector)
			{
			case slctShapefiles:
				return true;
			case slctIdentify:
				result = LayerIsIdentifiable(layerHandle, sf);
				break;
			case slctHotTracking:
				

				if (m_cursorMode == cmIdentify) 
				{
					VARIANT_BOOL hotTracking;
					_identifier->get_HotTracking(&hotTracking);
					if (hotTracking) 
						result = LayerIsIdentifiable(layerHandle, sf);
				}
				else {
					VARIANT_BOOL interactive;
					sf->get_InteractiveEditing(&interactive);

					tkLayerSelection highlighting;
					_shapeEditor->get_HighlightVertices(&highlighting);

					bool editorEmpty = EditorHelper::IsEmpty(_shapeEditor);
					bool editing = m_cursorMode == cmEditShape && !editorEmpty;
					bool selection = m_cursorMode == cmEditShape && editorEmpty;
					
					if (selection) 
					{
						// we are looking for new shape to select which can be at any editable layer
						if (interactive && highlighting != lsNoLayer)
							result = VARIANT_TRUE;
					}
					else if ((!editorEmpty && m_cursorMode == cmEditShape) ||
						EditorHelper::IsDigitizingCursor((tkCursorMode)m_cursorMode))
					{
						// highlight vertices for easier snapping
						switch (highlighting) {
							case lsAllLayers:
								return true;
							case lsActiveLayer:
								long handle;
								_shapeEditor->get_LayerHandle(&handle);
								return handle == layerHandle;
							case lsNoLayer:
								return false;
						}
					}
				}
				break;
			case slctInMemorySf:
				sf->get_EditingShapes(&result);
				break;
			case slctInteractiveEditing:
				sf->get_InteractiveEditing(&result);
				break;
			}
			return result ? true : false;
		}
	}
	return false;
}

// ************************************************************
//		GetMouseTolerance
// ************************************************************
// Mouse tolerance in projected units
double CMapView::GetMouseTolerance(MouseTolerance tolerance, bool proj)
{
	double tol = 0;
	switch(tolerance)
	{
		case ToleranceSnap:
			_shapeEditor->get_SnapTolerance(&tol);
			break;	
		case ToleranceSelect:
			tol = m_globalSettings.mouseTolerance;
			break;
		case ToleranceInsert:
			tol = 10;
			break;
	}
	if (proj)
		tol /= this->PixelsPerMapUnit();
	return tol;
}

// ************************************************************
//		DrillDownSelect
// ************************************************************
bool CMapView::DrillDownSelect(double projX, double projY, long& layerHandle, long& shapeIndex )
{
	vector<int> handles;
	SelectLayerHandles(slctIdentify, handles);
	for (int i = handles.size() - 1; i >= 0; i--)
	{
		CComPtr<IShapefile> sf = NULL;
		sf.Attach(GetShapefile(handles[i]));
		if (sf) {
			Extent box = GetPointSelectionBox(sf, projX, projY);
			
			if (SelectionHelper::SelectSingleShape(sf, box, shapeIndex))
			{
				layerHandle = handles[i];
				return true;
			}
		}
	}
	return false;
}

// ************************************************************
//		FindShapeAtScreenPoint
// ************************************************************
LayerShape CMapView::FindShapeAtScreenPoint(CPoint point, LayerSelector selector)
{
	double pixX = point.x;
	double pixY = point.y;
	double prjX, prjY;
	this->PixelToProj(pixX, pixY, &prjX, &prjY);

	std::vector<int> layers;
	if (SelectLayerHandles(selector, layers))
	{
		return FindShapeAtProjPoint(prjX, prjY, layers);
	}
	return LayerShape();
}

// ************************************************************
//		FindShapeCore
// ************************************************************
LayerShape CMapView::FindShapeAtProjPoint(double prjX, double prjY, std::vector<int>& layers)
{
	IShapefile * sf = NULL;
	for (int i = (int)layers.size() - 1; i >= 0; i--)
	{
		CComPtr<IShapefile> sf = NULL;
		sf.Attach(GetShapefile(layers[i]));
		if (sf) {
			double tol = 0.0;
			ShpfileType type = ShapefileHelper::GetShapeType2D(sf);
			if (type == SHP_MULTIPOINT || type == SHP_POINT || type == SHP_POLYLINE)
				tol = GetMouseTolerance(ToleranceSelect);

			SelectMode mode = type == SHP_POLYGON ? INCLUSION : INTERSECTION;

			long shapeIndex;
			if (SelectionHelper::SelectSingleShape(sf, Extent(prjX, prjY, tol), mode, shapeIndex)) {
				return LayerShape(layers[i], shapeIndex);
			}
		}
	}
	return LayerShape();
}

// ************************************************************
//		SelectShapeForEditing
// ************************************************************
bool CMapView::SelectShapeForEditing(int x, int y, long& layerHandle, long& shapeIndex)
{
	LayerShape info = FindShapeAtScreenPoint(CPoint(x, y), slctInteractiveEditing);
	if (!info.IsEmpty())
	{
		tkMwBoolean cancel = blnFalse;
		FireBeforeShapeEdit(info.LayerHandle, info.ShapeIndex, &cancel);
		layerHandle = info.LayerHandle;
		shapeIndex = info.ShapeIndex;
		return cancel == blnFalse;
	}
	return false;
}

// ************************************************************
//		RecalcHotTracking
// ************************************************************
HotTrackingResult CMapView::RecalcHotTracking(CPoint point, LayerShape& result)
{
	bool cursorCheck = EditorHelper::IsSnappableCursor((tkCursorMode)m_cursorMode) || m_cursorMode == cmEditShape || m_cursorMode == cmIdentify;
	if (!cursorCheck) return NoShape;

	if (_shapeCountInView < m_globalSettings.hotTrackingMaxShapeCount && HasHotTracking())
	{
		result = FindShapeAtScreenPoint(point, slctHotTracking);
		if (!result.IsEmpty()) {
			return _hotTracking.IsSame(result) ? SameShape : NewShape;
		}
	}
	return HotTrackingResult::NoShape;
}

// ************************************************************
//		ClearHotTracking
// ************************************************************
void CMapView::ClearHotTracking()
{
	if (!_hotTracking.IsEmpty())
	{
		_canUseMainBuffer = false;
		_hotTracking.Clear();
		this->FireShapeHighlighted(-1, -1);
	}
}

// ************************************************************
//		UpdateHotTracking
// ************************************************************
void CMapView::UpdateHotTracking(LayerShape info, bool fireEvent)
{
	CComPtr<IShapefile> sf = NULL;
	sf.Attach(GetShapefile(info.LayerHandle));
	if (sf) {
		CComPtr<IShape> shape = NULL;
		sf->get_Shape(info.ShapeIndex, &shape);
		if (shape) 
		{
			CComPtr<IShape> shpClone = NULL;
			shape->Clone(&shpClone);
			_hotTracking.Update(sf, shpClone, info.LayerHandle, info.ShapeIndex);

			OLE_COLOR color;
			_identifier->get_OutlineColor(&color);

			CComPtr<IShapeDrawingOptions> options = NULL;
			options.Attach(ShapeStyleHelper::GetHotTrackingStyle(sf, color, m_cursorMode == cmIdentify));
			if (options) {
				_hotTracking.UpdateStyle(options);
			}
			
			if (fireEvent)
				this->FireShapeHighlighted(_hotTracking.LayerHandle, _hotTracking.ShapeIndex);
			_canUseMainBuffer = false;
		}
	}
}

// **********************************************************
//			ClearDrawingLabelFrames()
// **********************************************************
void CMapView::ClearDrawingLabelFrames()
{
	// clear frames for drawing labels
	for (size_t j = 0; j < _activeDrawLists.size(); j++)
	{
		bool isSkip = false;
		for (size_t i = 0; i < _drawingLayerInvisilbe.size(); i++)
		{
			if (_drawingLayerInvisilbe[i] == j)
			{
				isSkip = true;	// skip if this layer is set invisible
				break;
			}
		}
		if (isSkip)
			continue;

		DrawList * dlist = _allDrawLists[_activeDrawLists[j]];
		if (IS_VALID_PTR(dlist))
		{
			CLabels* coLabels = static_cast<CLabels*>(dlist->m_labels);
			coLabels->ClearLabelFrames();
		}
	}
}

// **********************************************************
//			ClearLabelFrames()
// **********************************************************
void CMapView::ClearLabelFrames()
{
	// clear frames for regular labels
	for (int i = 0; i < (int)_activeLayers.size(); i++)
	{
		Layer * l = _allLayers[_activeLayers[i]];
		if( l != NULL )
		{	
			if (l->IsShapefile())
			{
				IShapefile * sf = NULL;
				if (l->QueryShapefile(&sf))
				{
					((CShapefile*)sf)->ClearChartFrames();
					sf->Release();
				}
			}
			
			// labels
			ILabels* LabelsClass = l->get_Labels();
			if (LabelsClass == NULL) continue;
			
			CLabels* coLabels = static_cast<CLabels*>(LabelsClass);
			coLabels->ClearLabelFrames();
			LabelsClass->Release(); LabelsClass = NULL;
		}
	}
}

#pragma endregion

