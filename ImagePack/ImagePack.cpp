// ImagePack.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "ImagePack.h"

#include <FreeImagePlus.h>
#include <CommonUtil/CommonUtil.h>

using namespace std;
using namespace Cactus;


String	g_strOutDir = ".";
bool	g_bBriefingName = false;
int		g_iFlag = 0;
int		g_iFlagOut = 0;


void FactorNumber(int iNumber, int& N1, int& N2)
{
	int iSqrt = sqrt(1.0 * iNumber);
	if (iSqrt * iSqrt != iNumber)
	{
		iSqrt ++;
	}
	else
	{
		N1 = iSqrt;
		N2 = iSqrt;
		return;
	}

	bool bPrime = true;
	int iQuotient = 0;
	for (int i = iSqrt; i >= 2; i--)
	{
		if (iNumber % i == 0)
		{
			bPrime = false;
			iQuotient = iNumber / i;

			if (i > iQuotient)
			{
				N1 = i;
				N2 = iQuotient;
			}
			else
			{
				N2 = i;
				N1 = iQuotient;
			}

			return;
		}
	}

	if (bPrime)
	{
		N1 = iNumber;
		N2 = 1;
	}
}


void PackFiles(fipImage* dest, const StringVector& vectFiles, int iW, int iH, int iX, int iY)
{
	int iCount = 0;

	for (size_t t = 0; t < vectFiles.size(); ++t)
	{
		fipImage* pImage0 = new fipImage;
		if (pImage0->load(vectFiles[t].c_str(), g_iFlag))
		{
			if( pImage0->getWidth() == iW
				&&
				pImage0->getHeight() == iH
				)
			{
				dest->pasteSubImage(*pImage0, iW * (iCount % iX), iH * (iCount / iX));

				iCount++;
			}
		}
		delete pImage0;
	}
}


void PackImageFolder(const Cactus::String& strRoot, const Cactus::String& strImageType, bool bRecursive)
{
	CFileFind finder;
	CString str = strRoot.c_str() + CString("/*.") + strImageType.c_str();

	Log_Debug("");
	Log_Debug("	Checking " << str);

	StringVector vectorFiles;

	// start working for files
	BOOL bWorking = finder.FindFile(str);
	while (bWorking)
	{
		bWorking = finder.FindNextFile();

		// skip . and .. files; otherwise, we'd recur infinitely!
		if (finder.IsDots())
			continue;

		// skip directory
		if (finder.IsDirectory())
			continue;

		Cactus::String strPath	= (LPCTSTR)finder.GetFilePath();
		vectorFiles.push_back(strPath);
	}
	finder.Close();

	if (vectorFiles.size() > 1)
	{
		Log_Debug("	Found images: " << vectorFiles.size());

		int iX, iY;
		FactorNumber(vectorFiles.size(), iX, iY);
		Log_Debug("	Image layout: " << iX << " x " << iY);

		int iUnitWidth = 0;
		int iUnitHeight = 0;
		int iBpp = 32;

		fipImage* pImage0 = new fipImage;
		if (pImage0->load(vectorFiles[0].c_str()))
		{
			iUnitWidth = pImage0->getWidth();
			iUnitHeight = pImage0->getHeight();

			if (strImageType == "jpg")
				iBpp = pImage0->getBitsPerPixel();
		}
		delete pImage0;

		Log_Debug("	Unit width: " << iUnitWidth);
		Log_Debug("	Unit height: " << iUnitHeight);
		Log_Debug("	Bit per pixel: " << iBpp);

		if (iUnitWidth * iUnitHeight)
		{
			fipImage* pImage = new fipImage(FIT_BITMAP, iUnitWidth * iX, iUnitHeight * iY, iBpp);
			
			PackFiles(pImage, vectorFiles, iUnitWidth, iUnitHeight, iX, iY);

			String strTmp = strRoot;
			StringVector outs = Split(strTmp, "\\/");
			String strImageName;
			int iSection = 0;
			if (outs.size() > 0)
			{
				if (g_bBriefingName)
				{
					strImageName = outs[outs.size() - 1];
				}
				else 
				{
					for (size_t t = 0; t < outs.size(); t++)
					{
						if (outs[t].length() == 2 && outs[t][1] == ':')
						{
							strImageName += outs[t][0];
							iSection++;
							continue;
						}

						if (outs[t] == ".")
							continue;

						if (iSection != 0)
							strImageName += "_";

						strImageName += outs[t];
						iSection++;
					}
				}
			}
			else
				strImageName = "ImagePack";

			strImageName = g_strOutDir + "/" + strImageName + "." + strImageType;

			if( pImage->save(strImageName.c_str(), g_iFlagOut) )
			{
				Log_Debug("	Big image generated successfully: " << strImageName);
			}
			else
			{
				Log_Error("	Big image generated failed: " << strImageName);
			}


			delete pImage;
		}

	}

	if (bRecursive)
	{
		CString str = strRoot.c_str() + CString("/*.*");

		BOOL bWorking = finder.FindFile(str);
		while (bWorking)
		{
			bWorking = finder.FindNextFile();

			// skip . and .. files; otherwise, we'd recur infinitely!
			if (finder.IsDots())
				continue;

			// manage directory
			if (finder.IsDirectory())
			{
				Cactus::String strPath	= (LPCTSTR)finder.GetFilePath();

				PackImageFolder(strPath, strImageType, bRecursive);
			}
		}
		finder.Close();
	}
}

/*
	-d DirectoryName -i png | jpg | bmp -r -o DirectoryName
*/
int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{

	CommandLineParser cmdParser;
	if( cmdParser.Parse(argc, argv) < 2 || !cmdParser.HasSwitch("-d") || !cmdParser.HasSwitch("-i") )
	{
		std::cout << "ImagePack 1.0" << endl;
		std::cout << "	A tool can pack small image files(same format and same pixel resolution) into a big image file." << std::endl;
		std::cout << "	Author: ZHK, zhk.tiger@gmail.com" << std::endl;
		std::cout << "Usage: " << endl << "	" << argv[0] << " -d DirectoryName -i png | jpg | bmp [-r] [-o DirectoryName] [-b]" << std::endl;
		std::cout << "	"			 << " -d DirectoryName  : specify the source directory name. Must provide." << std::endl;
		std::cout << "	"			 << " -i png | jpg | bmp: specify the image extension, you can try other format, like tif, gif, etc. Must provide." << std::endl;
		std::cout << "	"			 << " -r                : means check the subdirectory recursively. Optional, default NOT." << std::endl;
		std::cout << "	"			 << " -o DirectoryName  : specify the output directory. Optional, default is the exe directory." << std::endl;
		std::cout << "	"			 << " -b                : means use briefing file name for the output. Optional, default NOT." << std::endl;
		std::cout << "Example: " << endl << "	" << argv[0] << " -d d:\\MyImages -i png -r -o d:\\output" << std::endl;
		return 1;
	}

	Mem_New LogManager(eLogLevelDebug, "ImagePack.log");

	// Directory
	String	strDir = ".";
	strDir = cmdParser.GetArgument("-d");

	// Image Type
	String	strImageType = "png";
	strImageType = cmdParser.GetArgument("-i");
	if (strImageType == "jpg")
	{
		g_iFlag = JPEG_ACCURATE;
		g_iFlagOut = JPEG_QUALITYSUPERB;
	}


	bool bRecur = cmdParser.HasSwitch("-r");

	if ( cmdParser.HasSwitch("-o") )
	{
		g_strOutDir = cmdParser.GetArgument("-o");
	}

	if ( cmdParser.HasSwitch("-b") )
	{
		g_bBriefingName = true;
	}

	Log_Debug("Information:");
	Log_Debug("	Source directory: " << strDir);
	Log_Debug("	Image type      : " << strImageType);
	Log_Debug("	Output directory: " << g_strOutDir);
	Log_Debug("	Recursive       : " << (bRecur ? "True" : "False") );

	Log_Debug("Pack start...");

	PackImageFolder(strDir, strImageType, bRecur);

	Log_Debug("Pack end.");

	Mem_Delete LogManager::getSingletonPtr();

	return 0;
}
