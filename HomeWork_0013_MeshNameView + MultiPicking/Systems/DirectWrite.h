#pragma once

struct FontBrushDesc
{
	D3DXCOLOR Color;
	//여러가지 브러쉬가 많다
	ID2D1SolidColorBrush* Brush = NULL;

	bool operator==(const FontBrushDesc& val)
	{
		return Color.r == val.Color.r &&
			Color.g == val.Color.g &&
			Color.b == val.Color.b &&
			Color.a == val.Color.a;
	}
};

struct FontTextDesc
{
	wstring Font;
	float FontSize;

	DWRITE_FONT_WEIGHT Weight;
	DWRITE_FONT_STYLE Style;
	DWRITE_FONT_STRETCH Stretch;

	IDWriteTextFormat* Format = NULL;

	bool operator==(const FontTextDesc& val)
	{
		bool b = true;
		b &= Font == val.Font;
		b &= FontSize == val.FontSize;
		b &= Weight == val.Weight;
		b &= Style == val.Style;
		b &= Stretch == val.Stretch;

		return b;
	}
};

class DirectWrite
{
public:
	static void Create();
	static void Delete();

	static DirectWrite* Get();

	static ID2D1DeviceContext* GetDC() { return deviceContext; }

	static void CreateSurface();
	static void DeleteSurface();

	//


	static void CreateBitMap(LPCWSTR temp, ID2D1Bitmap** bitmap);
	static void DeleteBitMap();


public:
	static void RenderText
	(
		wstring text
		, RECT rect
		, float size = 20.0f
		, wstring font = L"돋움체"
		, D3DXCOLOR color = D3DXCOLOR(1, 1, 1, 1)
		, DWRITE_FONT_WEIGHT weight = DWRITE_FONT_WEIGHT_NORMAL
		, DWRITE_FONT_STYLE style = DWRITE_FONT_STYLE_NORMAL
		, DWRITE_FONT_STRETCH stretch = DWRITE_FONT_STRETCH_NORMAL
	);

	static void RenderImage(RECT rect, int Num);

private:
	static DirectWrite* instance;

private:
	DirectWrite();
	~DirectWrite();

private:
	static ID2D1Factory1* factory;
	static IDWriteFactory* writeFactory;

	static ID2D1Device* device;
	static ID2D1DeviceContext* deviceContext;

	static ID2D1Bitmap1* targetBitmap;
	static IDXGISurface* dxgiSurface; //글씨 쓰는 백버퍼 역할

	// png파일 읽어서 화면출력


	//[ IWICBitmapDecoder 객체 생성 ]

	// WIC(Windows Imaging Component)관련 객체를 생성하기 위한 Factory 객체 선언 
	static IWICImagingFactory *p_wic_factory;
	// 압축된 이미지를 해제할 객체 
	static IWICBitmapDecoder *p_decoder;

	static wstring Temp;
	/*
	std::wstring stemp = std::wstring(s.begin(), s.end());
	LPCWSTR sw = stemp.c_str();
	*/

	// [IWICBitmapFrameDecode 객체 생성] 특정그림

	static IWICBitmapFrameDecode *p_frame;

	// [ Direct2D에서 사용할 수 있도록 이미지 변환하기 ]

	// 이미지 변환 객체 
	static IWICFormatConverter *p_converter;

	// Direct2D의 기본 render target에서 사용가능한 기본 비트맵 객체 
	static ID2D1Bitmap *p_bitmap;
	static ID2D1Bitmap *p_bitmap2;



private:
	static vector<FontBrushDesc> fontBrushes;
	static vector<FontTextDesc> fontTexts;
};