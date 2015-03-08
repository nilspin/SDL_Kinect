// SDL_Kinect.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

int main(int argc, char *argv[])//_tmain(int argc, _TCHAR* argv[])////
{

/*Random number stuff*/
	std::random_device rd;//obtain random number from device
	std::mt19937 eng(rd());//seed the generator
	std::uniform_int_distribution<> distr(0, 255);


/*Kinect stuff*/
	IKinectSensor* sensor;

	HRESULT hResult = S_OK;
	hResult = GetDefaultKinectSensor(&sensor);
	if (FAILED(hResult)){
		std::cerr << "Error : GetDefaultKinectSensor" << std::endl;
		return -1;
	}

	hResult = sensor->Open();
	if (FAILED(hResult)){
		std::cerr << "Error : IKinectSensor::Open()" << std::endl;
		return -1;
	}

	//Source
	IDepthFrameSource* IR_Source;
	hResult = sensor->get_DepthFrameSource(&IR_Source);
	if (FAILED(hResult)){
		std::cerr << "Error : IKinectSensor::get_InfraredFrameSource()" << std::endl;
		return -1;
	}

	// Reader
	IDepthFrameReader* IR_Reader;
	hResult = IR_Source->OpenReader(&IR_Reader);
	if (FAILED(hResult)){
		std::cerr << "Error : IInfraredFrameSource::OpenReader()" << std::endl;
		return -1;
	}

	// Description
	IFrameDescription* frameDescription;
	hResult = IR_Source->get_FrameDescription(&frameDescription);
	if (FAILED(hResult)){
		std::cerr << "Error : IInfraredFrameSource::get_FrameDescription()" << std::endl;
		return -1;
	}

	int width; frameDescription->get_Width(&width);
	int height; frameDescription->get_Height(&height);

/*SDL init stuff*/
	bool quit = false;
	SDL_Init(SDL_INIT_EVERYTHING); //init
	SDL_Event event; //event handler
	SDL_Window *window = SDL_CreateWindow("Kinect sample app", 300, 200, width, height, SDL_WINDOW_RESIZABLE | SDL_WINDOW_SHOWN);//window
	
	SDL_Surface *surface = SDL_GetWindowSurface(window);

/*Main loop*/

	while (!quit)
	{
		while (SDL_PollEvent(&event) != 0)
		{
			if (event.type == SDL_QUIT || event.type == SDLK_ESCAPE) quit = true;
		}

		IDepthFrame *IR_Frame = NULL;
		hResult = IR_Reader->AcquireLatestFrame(&IR_Frame);
		if (!FAILED(hResult))
		{
			unsigned int bufferSize = 0;
			unsigned short* buffer = nullptr;
			Uint8 rnum;
			hResult = IR_Frame->AccessUnderlyingBuffer(&bufferSize, &buffer);
			if (!FAILED(hResult))
			{
				SDL_ConvertPixels(width, height, SDL_PIXELFORMAT_RGB565, buffer, width * 2, surface->format->format, surface->pixels, surface->pitch);
			}
			SDL_UpdateWindowSurface(window);
			SafeRelease(IR_Frame);
		}
	}

	SafeRelease(frameDescription);
	SafeRelease(IR_Reader);
	SafeRelease(IR_Source);
	sensor->Close();
	SafeRelease(sensor);
	SDL_FreeSurface(surface);
	SDL_DestroyWindow(window);

	return 0;
}