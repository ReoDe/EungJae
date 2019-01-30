#pragma once

struct FontBrushDesc
{
	D3DXCOLOR Color;
	//�������� �귯���� ����
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
		, wstring font = L"����ü"
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
	static IDXGISurface* dxgiSurface; //�۾� ���� ����� ����

	// png���� �о ȭ�����


	//[ IWICBitmapDecoder ��ü ���� ]

	// WIC(Windows Imaging Component)���� ��ü�� �����ϱ� ���� Factory ��ü ���� 
	static IWICImagingFactory *p_wic_factory;
	// ����� �̹����� ������ ��ü 
	static IWICBitmapDecoder *p_decoder;

	static wstring Temp;
	/*
	std::wstring stemp = std::wstring(s.begin(), s.end());
	LPCWSTR sw = stemp.c_str();
	*/

	// [IWICBitmapFrameDecode ��ü ����] Ư���׸�

	static IWICBitmapFrameDecode *p_frame;

	// [ Direct2D���� ����� �� �ֵ��� �̹��� ��ȯ�ϱ� ]

	// �̹��� ��ȯ ��ü 
	static IWICFormatConverter *p_converter;

	// Direct2D�� �⺻ render target���� ��밡���� �⺻ ��Ʈ�� ��ü 
	static ID2D1Bitmap *p_bitmap;
	static ID2D1Bitmap *p_bitmap2;



private:
	static vector<FontBrushDesc> fontBrushes;
	static vector<FontTextDesc> fontTexts;
};