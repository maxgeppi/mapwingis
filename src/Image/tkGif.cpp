// tkGif.cpp: implementation of the tkGif class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "tkGif.h"
#include <cassert>

extern "C" 
{
#include "cq.h"
}

using namespace std;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

const int MASKTABLE[] = { 0x0000, 0x0001, 0x0003, 0x0007,
                      0x000f, 0x001f, 0x003f, 0x007f,
					  0x00ff, 0x01ff, 0x03ff, 0x07ff,
					  0x0fff, 0x1fff, 0x3fff, 0x7fff };
const int SHIFTINC[] = { 0, 1, 2, 3, 4, 5, 6, 7, 0, 1, 2, 3, 4, 5 };
const int BITSLEFT[] = { 8, 7, 6, 5, 4, 3, 2, 1, 0 };
const int NUMREADINC[] = { 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1 };

bool tkGif::ReadGif(const char * filename)
{
	GIFHEADER gh;
	GIFIMAGEBLOCK iblk;

	int bits;
	int blocktype;

	if((fp=fopen(filename, "rb"))==NULL) 
		return false;

	fread(&gh, 1, sizeof(GIFHEADER), fp);

	if(memcmp(gh.sig, "GIF", 3)) // is it not a GIF file
	{
		fclose(fp);
		return false;
	}

	bits = (gh.flags & 7) + 1;
	if(gh.flags & 0x80) 
		ReadPalette(bits);

	while((blocktype = fgetc(fp)) != EOF) {

		if(blocktype == IMAGEBLOCK) {
			fread(&iblk, 1, sizeof(GIFIMAGEBLOCK), fp);
			height = iblk.height;
			width = iblk.width;
			buffSize = width * height;
			
			buffer = new colort[buffSize];

			if(iblk.flags & 0x80) ReadPalette((iblk.flags & 7) + 1);

			isInterlaced = iblk.flags & 0x40;

			bits = fgetc(fp);

			DecompressGIF(bits);
			fclose(fp);
			FlipBuffer();
			if(hasTransparency) transColor = ColorMap[transColor].c;
			//WriteGif("d:\\savetest.gif");

			for (int zi = 0; zi < buffSize; zi++)
			{
				// Flip red and blue
				unsigned char tmp = buffer[zi].b;
				buffer[zi].b = buffer[zi].r;
				buffer[zi].r = tmp;
			}

			return true;
		}
		else if(blocktype == EXTENSIONBLOCK) {
			SkipExtension(); // excepts now it gets the transparent color instead of skipping
		}
	}

	fclose(fp);
	return false;
}

bool tkGif::WriteGif(const char *filename)
{
	GIFHEADER gh;
	ZeroMemory(&gh, sizeof(gh));

	// WRite signature
	gh.sig[0]='G';
	gh.sig[1]='I';
	gh.sig[2]='F';
	gh.sig[3]='8';
	gh.sig[4]='9';
	gh.sig[5]='a';

	gh.screenheight = height;
	gh.screenwidth = width;
	gh.flags |= 0xF7; // There's  a global colormap and it has 8 bits;

	ColorMapSize = 0;
	ZeroMemory(&ColorMap, sizeof(ColorEntry) << 12);

	FlipBuffer();

	Quantize();
	
	fp = fopen(filename, "wb");
	if(fp == NULL) return false;

	fwrite(&gh, 1, sizeof(GIFHEADER), fp);
	WritePalette();

	// Write the GIF Control Block
	GIFCONTROLBLOCK cb;
	ZeroMemory(&cb, sizeof(GIFCONTROLBLOCK));
	cb.blocksize = 4;
	if(hasTransparency)
	{
		cb.flags = 1;
		int i = 0;
		for(i = 0; i < 256; i++)
			if(ColorMap[i].c == transColor) break;
		cb.transparent_color = i;
	}

	fputc(EXTENSIONBLOCK, fp);
	fputc(CONTROLBLOCK,fp);
	fwrite(&cb, 1, sizeof(GIFCONTROLBLOCK), fp);

	fputc(IMAGEBLOCK, fp);

	GIFIMAGEBLOCK iblk;
	iblk.flags =  7;
	iblk.height = height;
	iblk.left = 0;
	iblk.top = 0;
	iblk.width = width;

	fwrite(&iblk, 1, sizeof(GIFIMAGEBLOCK), fp);
	fputc(8,fp);

	CompressGif();
	fputc(0x3b,fp);
	fclose(fp);

	height = gh.screenheight;
	width = gh.screenwidth;

	return true;

}

void tkGif::Quantize()
{
	size = buffSize;
	K = 256;

	Ir = (unsigned char *)malloc(size);
	Ig = (unsigned char *)malloc(size);
	Ib = (unsigned char *)malloc(size);

	int i = 0;
	for(i = 0; i < size; i++)
	{
		Ir[i] = buffer[i].r;
		Ig[i] = buffer[i].g;
		Ib[i] = buffer[i].b;
	}
	
	main_cq();

	for(i = 0; i < 256; i++)
	{
		ColorMap[i].c.r = lut_r[i];
		ColorMap[i].c.g = lut_g[i];
		ColorMap[i].c.b = lut_b[i];
	}

	ColorMapSize = i + 1;

	for(i = 0; i < size; i++)
	{
		buffer[i].r = lut_r[Qadd[i]];
		buffer[i].g = lut_g[Qadd[i]];
		buffer[i].b = lut_b[Qadd[i]];
	}

	free(Qadd);
	Qadd = NULL;
}

void tkGif::DecompressGIF(int bits)
{
	int clear_code = 1 << bits; // the code to tell to clear the ColorMap
	int eof_code = clear_code + 1; // code to tell the image is done
	int code, old_code;
	codesize = bits+1;
	int bitsleft = codesize;
	int max_code = clear_code << 1;

	int plotloc = 0;
	int base_code = 0;

	maxcode = clear_code;
	//ofstream fout("d:\\ReadCodes.txt");
	//int codecount = 0;

	do {
		code = GetCode();
		//fout << codecount++ << ":\t" << code << endl;

		if( code != eof_code )
		{
			if( code == clear_code )
			{
				codesize = bits + 1;
				max_code = (1 << codesize) - 1;
				memset(&ColorMap[clear_code], 0, 4960-clear_code);
				code = GetCode();
				//fout << codecount++ << ":\t" << code << endl;
				old_code = code;
				maxcode = eof_code;

				plot(&ColorMap[code]);
			}
			else
			{
				//Does the code exist
				if ( code <= maxcode )
				{
					plot(&ColorMap[code]);

					// New Entry to Table
					maxcode++;
					ColorMap[maxcode].next = &ColorMap[old_code];
					ColorMap[maxcode].c = basecolor(&ColorMap[code]);

					old_code = code;
				}
				else
				{
					assert(code <= (maxcode + 1) && "code is too big!");
					maxcode++;
					ColorMap[maxcode].next = &ColorMap[old_code];
					ColorMap[maxcode].c = basecolor(&ColorMap[old_code]);

					plot(&ColorMap[code]);
					old_code = code;
				}
			}
			if( maxcode >= max_code )
			{
				if(codesize < 12)
				{
					codesize++;
					max_code <<= 1;
					max_code++;
				}
			}
		}
				
		
	} while (code != eof_code);

	if(isInterlaced) DeInterlace();
}

inline int tkGif::GetCode()
{
	int code; // the code to return

	if(numread >= blocksize - 2)
	{
		if(blocksize==numread)
		{
			blocksize = fgetc(fp);
			fread(block, 1, blocksize, fp);
			bitshift = SHIFTINC[codesize];
			numread = 0;
			code = (block[1] << 8) | block[0];
			numread += NUMREADINC[codesize];
		}
		else if (blocksize==numread+1)
		{
			code = block[numread];

			blocksize = fgetc(fp);
			fread(block, 1, blocksize, fp);

			numread = 0;
			code |= block[numread] << 8 | block[numread+1] << 16;
			code >>= bitshift;

			bitshift += SHIFTINC[codesize];
		}
		else
		{
			if((codesize - BITSLEFT[bitshift] - 1) >> 3)
			{
				code = (block[numread+1] << 8) | block[numread];
				blocksize = fgetc(fp);
				fread(block, 1, blocksize, fp);
				numread = 0;
				code |= block[0] << 16;
				code >>= bitshift;
				bitshift += SHIFTINC[codesize];
				bitshift %= 8;
			}
			else
			{
				code = (block[numread+1] << 8) | block[numread];
				numread += NUMREADINC[codesize];
				code >>= bitshift;
				bitshift += SHIFTINC[codesize];
			}
		}

	}
	else
	{
		code = (block[numread+2] << 16) | (block[numread+1] << 8) | block[numread];
		numread += NUMREADINC[codesize];
		code >>= bitshift;
		bitshift += SHIFTINC[codesize];
	}

	if(bitshift>=8) {bitshift %= 8; numread++;}

	return code & MASKTABLE[codesize];
}

void tkGif::SkipExtension()
{
	GIFPLAINTEXT pt;
	GIFCONTROLBLOCK cb;
	GIFAPPLICATION ap;
	int c, n;

	c = fgetc(fp);

	switch(c) {

	case PLAINTEXTBLOCK:
		if(fread(&pt, 1, sizeof(GIFPLAINTEXT), fp) == sizeof(GIFPLAINTEXT)) {
			do {
				n = 0;
				if((n=fgetc(fp)) != EOF)
					fseek(fp, n, SEEK_CUR);
			} while(n > 0);
		}
		break;

	case CONTROLBLOCK:
		fread(&cb, 1, sizeof(GIFCONTROLBLOCK), fp);
		if(cb.flags & 0x01) // Has a transparency color
		{
			transColor = cb.transparent_color; // Get the Transparent Color
			hasTransparency = true;
		}
		
		break;

	case APPLICATIONBLOCK:
		if(fread(&ap, 1, sizeof(GIFAPPLICATION), fp) == sizeof(GIFAPPLICATION)) {
			do {
				n = 0;
				if((n=fgetc(fp)) != EOF)
					fseek(fp, n, SEEK_CUR);
			} while(n > 0);
		}
		break;

	default:
		do {
			n = 0;
			if((n = fgetc(fp)) != EOF)
				fseek(fp, n, SEEK_CUR);
		} while (n > 0);
	}
}


void tkGif::ReadPalette(int bits)
{
	ColorMapSize = 1 << bits;

	for(int i = 0; i < ColorMapSize; i++)
		fread(&ColorMap[i].c, 1, 3, fp);

}

void tkGif::DeInterlace()
{
	int linesize = width * 3;
	int max = buffSize * 3;  // the size of the buffer in bytes

	colort * tmp = new colort[buffSize];
	memcpy(tmp, buffer, max);
	ZeroMemory(buffer, max);

	int i;
	int jump;
	colort * curline;
	colort * bufferline;

	//DeInterlace Frame 1;
	jump = width << 3;
	bufferline = buffer;
	curline = tmp;

	for(i = 0; i < height; i += 8)
	{
		memcpy(bufferline, curline, linesize);
		bufferline += jump;
		curline += width;
	}

	//DeInterlace Frame 2;
	bufferline = buffer + (width<<2);

	for(i = 4; i < height; i += 8)
	{
		memcpy(bufferline, curline, linesize);
		bufferline += jump;
		curline += width;
	}

	//DeInterlace Frame 3;
	jump = width << 2;
	bufferline = buffer + (width<<1);

	for(i = 2; i < height; i += 4)
	{
		memcpy(bufferline, curline, linesize);
		bufferline += jump;
		curline += width;
	}

	//DeInterlace Frame 4;
	jump = width << 1;
	bufferline = buffer + width;

	for(i = 1; i < height; i += 2)
	{
		memcpy(bufferline, curline, linesize);
		bufferline += jump;
		curline += width;
	}

	delete [] tmp;
}

unsigned char * tkGif::toDib()
{
	int dibwidth = width*3;
	int linewidth = dibwidth;

	dibwidth += (4 - dibwidth) & 3;

	int max = buffSize * 3; // size of buffer in bytes

	unsigned char * dib = new unsigned char[max];
	
	colort * tmp = new colort[width];
	unsigned char * db = dib;
	colort * tb = tmp;
	colort * bb = buffer;

	// REVERSE RGB
	for(int j = 0; j < height; j++)
	{
		for(int i = 0; i < width; i++)
		{
			tmp[i].b = bb[i].r;
			tmp[i].g = bb[i].g;
			tmp[i].r = bb[i].b;
		}

		memcpy(db, tmp, linewidth);
		db += dibwidth;
		bb += width;
	}

	delete [] tmp;

	return dib;
}

inline bool tkGif::inColorMap(colort c)
{
	for(int i = 0; i < ColorMapSize; i++)
		if(!memcmp(&ColorMap[i].c,&c,3)) return true;

	return false;
}

inline void tkGif::WritePalette()
{
	//for(int i = 0; i < 256; i++)
	//	fwrite(&ColorMap[i].c,1,3,fp);
	// Chris Michaelis, 11/3/2006 -- for BugZilla 333, swapped image
	// colors for GIF images
	for(int i = 0; i < 256; i++)
	{
		int nc = (&ColorMap[i].c)->b | (&ColorMap[i].c)->g << 8 | (&ColorMap[i].c)->r << 16;
		fwrite(&nc,1,3,fp);
	}
}

void tkGif::CompressGif()
{
	int eof_code = 257;
	int clear_code = 256;
	codesize = 9;
	max_code = 511;
	ht = new HashTable();
	ht->InitTable(ColorMap, ColorMapSize);

	maxcode = 257;
	bitshift = 0;
	numread = 0;
	blocksize = 255;
	plotloc = 0;

	ZeroMemory(&block, 256);
	
	colort * curcolor = buffer;
	colort * endcolor = &buffer[buffSize+1];

	int pre = ht->Find(*curcolor++, NULL);

	int colorcode;
	int curpos = 0;
	long percent, oldpercent = 0;

	char* msg = "Compressing Image";

	PutCode(clear_code);

	for(;curcolor < endcolor; curcolor++)
	{
		colorcode = ht->Find(*curcolor, ColorMap+pre);

		if(colorcode >= 0)
		{
			pre = colorcode;
		}
		else
		{
			PutCode(pre);
			pre = AddEntry(pre, *curcolor);
		}

		curpos++;
		CallbackHelper::Progress(cBack, curpos, buffSize, msg, key, percent);
	}

	PutCode(eof_code);
	blocksize = numread+1;
	fputc(blocksize, fp);
	fwrite(block,1,blocksize,fp);
	fputc(0,fp);

	CallbackHelper::ProgressCompleted(cBack, key);
}

inline int tkGif::AddEntry(int prefix, colort c)
{
	maxcode++;
	
	if(maxcode - 4096) 
	{
		ColorMap[maxcode].next = &ColorMap[prefix];
		ColorMap[maxcode].c = c;

		ht->InsertEntry(&ColorMap[maxcode], maxcode);
	}

	// Faster but the compression is a little less
	/*if( maxcode > max_code )
	{
		codesize++;
		max_code <<= 1;
		max_code++;

		if(codesize==12)
		{
			PutCode(256);
			codesize = 9;
			max_code = 511;
			maxcode = 257;
			ht->Clear();
			ht->InitTable(ColorMap, 256);
		}
	}*/

	// This one has a slightly better compression
	if( maxcode > max_code )
	{
		if(codesize < 12)
		{
			codesize++;
			max_code <<= 1;
			max_code++;
		}
		else
		{
			PutCode(256);
			codesize = 9;
			max_code = 511;
			maxcode = 257;
			ht->Clear();
			ht->InitTable(ColorMap, 256);
		}
	}

	return ht->Find(c,NULL);
}

inline void tkGif::PutCode(int code)
{
	//static ofstream fout("d:\\WriteCodes.txt");
	//static int codecount = 0;

	//if(codecount == 263)
	//	code = code;

	//fout << codecount++ << ":\t" << code << endl;
	block[numread] |= code << bitshift; numread++; code >>= BITSLEFT[bitshift];

	if(numread >= blocksize)
	{
		//OutPut Block
		fputc(blocksize, fp);
		fwrite(block,1,blocksize,fp);

		//Start new block
		ZeroMemory(&block, 256);
		numread = 0;
	}
	
	block[numread] = code;
	bitshift += SHIFTINC[codesize];

	if(bitshift>7)
	{
		bitshift &= 7;
		numread++;
		if(numread >= blocksize)
		{
			//OutPut Block
			fputc(blocksize, fp);
			fwrite(block,1,blocksize,fp);

			//Start new block
			ZeroMemory(&block, 256);
			numread = 0;
		}
		if(bitshift) block[numread] = code >> 8;
	}
}


bool tkGif::WritePPM(CStringW filename)
{
	FlipBuffer();

	ofstream outfile(filename, ios::binary);

	outfile << "P6\n" << "# PPM Created By tkImage.dll\n";
	outfile << width << ' ' << height << ' ' << "255\n";

	outfile.write((char *)buffer, buffSize*3);

	outfile.close();

	return true;
}

bool tkGif::ReadPPM(const char *filename)
{
	ifstream infile(filename,ios::binary);
	if(infile.eof()) return false;
	char firstline[20];
	char secondline[80];
	int depth;

	infile.getline(firstline,20);
	infile.getline(secondline,80);

	infile >> width >> height >> depth;

	infile.ignore();

	buffSize = width * height;

	if(buffer) delete [] buffer;
	buffer = new colort[buffSize];

	infile.read((char *)buffer, buffSize*3);

	infile.close();
	FlipBuffer();
	return true;
}


void tkGif::FlipBuffer()
{
	int linesize = width * 3; // the size of a line in bytes
	colort * tmp = new colort[buffSize]; // a tmp buffer to assist in the flipping
	memcpy(tmp, buffer, buffSize * 3);

	colort * bp = buffer;
	colort * tp = &tmp[buffSize - width];

	for( int i = 0; i < height; i++)
	{
		memcpy(bp, tp, linesize);
		tp -= width;
		bp += width;
	}

	delete [] tmp;
}

void tkGif::getPalette()
{
	ColorMapSize = 0;

	for(int i = 0; i < buffSize && ColorMapSize < 256; i++)
	{
		if(!inColorMap(buffer[i]))
		{
			ColorMap[ColorMapSize++].c = buffer[i];
		}
	}

}


