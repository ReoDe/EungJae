#include "stdafx.h"
#include "DirectWrite.h"

DirectWrite* DirectWrite::instance = NULL;

ID2D1DeviceContext* DirectWrite::deviceContext = NULL;
IDWriteFactory* DirectWrite::writeFactory = NULL;
ID2D1Bitmap1* DirectWrite::targetBitmap = NULL;
IDXGISurface* DirectWrite::dxgiSurface = NULL;

vector<FontBrushDesc> DirectWrite::fontBrushes;
vector<FontTextDesc> DirectWrite::fontTexts;

//

IWICImagingFactory* DirectWrite::p_wic_factory = NULL;
IWICBitmapDecoder * DirectWrite::p_decoder = NULL;

wstring DirectWrite::Temp = L"";

IWICBitmapFrameDecode* DirectWrite::p_frame = NULL;
IWICFormatConverter* DirectWrite::p_converter = NULL;

ID2D1Bitmap* DirectWrite::p_bitmap = NULL;
ID2D1Bitmap* DirectWrite::p_bitmap2 = NULL;
//

ID2D1Factory1* DirectWrite::factory = NULL;
ID2D1Device* DirectWrite::device = NULL;

void DirectWrite::Create()
{
	assert(instance == NULL);

	instance = new DirectWrite();
}

void DirectWrite::Delete()
{
	SAFE_DELETE(instance);
}

DirectWrite * DirectWrite::Get()
{
	assert(instance != NULL);

	return instance;
}

DirectWrite::DirectWrite()
{
	HRESULT hr;

	hr = DWriteCreateFactory
	(
		DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), (IUnknown **)&writeFactory
	);
	assert(SUCCEEDED(hr));

	D2D1_FACTORY_OPTIONS option;
	option.debugLevel = D2D1_DEBUG_LEVEL_NONE;
	hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_MULTI_THREADED, &factory);
	assert(SUCCEEDED(hr));

	IDXGIDevice* dxgiDevice;
	hr = D3D::GetDevice()->QueryInterface(&dxgiDevice);
	assert(SUCCEEDED(hr));

	hr = factory->CreateDevice(dxgiDevice, &device);
	assert(SUCCEEDED(hr));

	hr = device->CreateDeviceContext
	(
		D2D1_DEVICE_CONTEXT_OPTIONS_ENABLE_MULTITHREADED_OPTIMIZATIONS, &deviceContext
	);
	assert(SUCCEEDED(hr));

	CreateSurface();


	CreateBitMap(L"Red.png", &p_bitmap);

	CreateBitMap(L"Blue.png", &p_bitmap2);
}

DirectWrite::~DirectWrite()
{
	DeleteSurface();

	SAFE_RELEASE(factory);
	SAFE_RELEASE(writeFactory);
	SAFE_RELEASE(deviceContext);
	SAFE_RELEASE(device);
	//

	DeleteBitMap();

}

void DirectWrite::CreateSurface()
{
	//                              0번버퍼는 3d화면
	HRESULT hr = D3D::GetSwapChain()->GetBuffer(0, __uuidof(IDXGISurface), (void **)&dxgiSurface);
	assert(SUCCEEDED(hr));

	D2D1_BITMAP_PROPERTIES1 bp;
	bp.pixelFormat.format = DXGI_FORMAT_R8G8B8A8_UNORM;
	bp.pixelFormat.alphaMode = D2D1_ALPHA_MODE_IGNORE;
	bp.dpiX = 96; //도트퍼 인치 1인치에 몇개의 도트가 들어가나
	bp.dpiY = 96;
	bp.bitmapOptions = D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW;
	bp.colorContext = NULL;

	hr = deviceContext->CreateBitmapFromDxgiSurface(dxgiSurface, &bp, &targetBitmap);
	bp.colorContext = NULL;

	deviceContext->SetTarget(targetBitmap);
}

void DirectWrite::DeleteSurface()
{
	deviceContext->SetTarget(NULL);

	for (FontBrushDesc desc : fontBrushes)
		SAFE_RELEASE(desc.Brush);

	fontBrushes.clear();

	for (FontTextDesc desc : fontTexts)
		SAFE_RELEASE(desc.Format);

	fontTexts.clear();

	SAFE_RELEASE(dxgiSurface);
	SAFE_RELEASE(targetBitmap);

	/*if (p_bitmap != NULL)
		DeleteBitMap();*/

}

void DirectWrite::CreateBitMap(LPCWSTR temp, ID2D1Bitmap** bitmap)
{
	// WIC 객체를 생성하기 위한 Factory 객체를 생성한다. 
	CoCreateInstance(CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&p_wic_factory));


	// WIC용 Factory 객체를 사용하여 이미지 압축 해제를 위한 객체를 생성 
	p_wic_factory->CreateDecoderFromFilename(temp, NULL, GENERIC_READ, WICDecodeMetadataCacheOnDemand, &p_decoder);
	
	p_decoder->GetFrame(0, &p_frame); // png파일중 0번째 프레임

	 // IWICBitmap형식의 비트맵을 ID2D1Bitmap. 형식으로 변환하기 위한 객체 생성 
	p_wic_factory->CreateFormatConverter(&p_converter);


	// 선택된 그림을 어떤 형식의 비트맵으로 변환할 것인지 설정한다. 
	p_converter->Initialize(p_frame, GUID_WICPixelFormat32bppPBGRA, WICBitmapDitherTypeNone,
		NULL, 0.0f, WICBitmapPaletteTypeCustom);

	//이렇게 설정이 끝났다면 IWICBitmap 형식의 이미지를 가지고 ID2D1Bitmap 객체를 생성합니다.

	// IWICBitmap 형식의 비트맵으로 ID2D1Bitmap 객체를 생성한다. 

	HRESULT hr = deviceContext->CreateBitmapFromWicBitmap(p_converter, NULL, &(*bitmap));
	assert(SUCCEEDED(hr));



	

	deviceContext->SetTarget(*bitmap);


}

void DirectWrite::DeleteBitMap()
{

	SAFE_RELEASE(p_wic_factory);
	SAFE_RELEASE(p_decoder);
	SAFE_RELEASE(p_frame);
	SAFE_RELEASE(p_converter);
	SAFE_RELEASE(p_bitmap);
}

void DirectWrite::RenderText(wstring text, RECT rect, float size, wstring font, D3DXCOLOR color, DWRITE_FONT_WEIGHT weight, DWRITE_FONT_STYLE style, DWRITE_FONT_STRETCH stretch)
{
	FontBrushDesc brushDesc;
	brushDesc.Color = color;

	FontBrushDesc* findBrush = NULL;
	for (FontBrushDesc& desc : fontBrushes)
	{
		if (desc == brushDesc)
		{
			findBrush = &desc;
			break;
		}
	}

	if (findBrush == NULL)
	{
		D2D1::ColorF colorF = D2D1::ColorF(color.r, color.g, color.b);
		deviceContext->CreateSolidColorBrush(colorF, &brushDesc.Brush);

		fontBrushes.push_back(brushDesc);
		findBrush = &brushDesc;
	}

	FontTextDesc textDesc;
	textDesc.Font = font;
	textDesc.FontSize = size;
	textDesc.Stretch = stretch;
	textDesc.Style = style;
	textDesc.Weight = weight;

	FontTextDesc* findText = NULL;
	for (FontTextDesc& desc : fontTexts)
	{
		if (desc == textDesc)
		{
			findText = &desc;
			break;
		}
	}

	if (findText == NULL)
	{
		writeFactory->CreateTextFormat
		(
			textDesc.Font.c_str(), NULL, textDesc.Weight, textDesc.Style, textDesc.Stretch, textDesc.FontSize,
			L"ko", &textDesc.Format
		);

		fontTexts.push_back(textDesc);
		findText = &textDesc;
	}

	D2D1_RECT_F rectF;
	rectF.left = (float)rect.left;
	rectF.right = (float)rect.right;
	rectF.top = (float)rect.top;
	rectF.bottom = (float)rect.bottom;

	deviceContext->DrawTextW(text.c_str(), text.size(), findText->Format, rectF, findBrush->Brush);
}

void DirectWrite::RenderImage(RECT rect, int Num)
{

	D2D1_RECT_F rectF;
	rectF.left = (float)rect.left;
	rectF.right = (float)rect.right;
	rectF.top = (float)rect.top;
	rectF.bottom = (float)rect.bottom;

	if (Num == 1)
		deviceContext->DrawBitmap(p_bitmap, rectF);
	if (Num == 2)
		deviceContext->DrawBitmap(p_bitmap2, rectF);

}

