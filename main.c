#include <stdio.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <windows.h>

const int SCREENSIZEX=480;
const int SCREENSIZEY=720;

const int FPS=60;

TTF_Font* Fonts[4];
SDL_Window* mainwind = NULL;
SDL_Window* newlaunchwind = NULL;
SDL_Renderer* rend = NULL;
SDL_Renderer* newlaunchrend = NULL;
SDL_Texture* ButtonsTex = NULL;
SDL_Texture* LogoTex = NULL;
SDL_Texture* MainBackgr = NULL;

void prntdbg(const char* arr)
{
	printf("  [%d]%s\n", SDL_GetTicks(), arr);
}

bool init()
{
	prntdbg("Enter init");
	bool success = true;
	prntdbg("SDL_Init( SDL_INIT_EVERYTHING ) started");
	if( SDL_Init( SDL_INIT_EVERYTHING ) < 0 )
	{
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
		success = false;
	}
	else
	{
		prntdbg("SDL_Init( SDL_INIT_EVERYTHING ) DONE");
		Uint32 windflg;
		windflg = SDL_WINDOW_SHOWN;
		prntdbg("Creating window");
		mainwind = SDL_CreateWindow("Orbita client v0.0.0.0.1pre-alpha test build 7", 
								SDL_WINDOWPOS_CENTERED, 
								SDL_WINDOWPOS_CENTERED, 
								480, 
								720, 
								windflg);
		if( mainwind == NULL ) {
			printf("Window could not be created! SDL_Error: %s\n", SDL_GetError() );
			success = false;
		}
		else {
			prntdbg("Creating window done");
			prntdbg("Creating render");
			rend = SDL_CreateRenderer(mainwind, -1, SDL_RENDERER_ACCELERATED);
			if(rend == NULL) {
				printf("rend err\n");
				success = false;
			}
			else {
				prntdbg("Creating render done");
				SDL_SetRenderDrawColor(rend, 255, 255, 255, 255);
				int imgFlags = IMG_INIT_PNG;
				prntdbg("Initializing SDL Image");
				if(!(IMG_Init(imgFlags)&imgFlags))
				{
					printf("Err: init sdlimg %s\n", IMG_GetError());
					success=false;
				}
				else
				{
					prntdbg("Initializing SDL Image done");
					prntdbg("Initializing SDL TTF");
					if(TTF_Init() < 0)
					{
						printf("SDL TTF %s\n",TTF_GetError());
						success=false;
					}
					else
					{
						prntdbg("Initializing SDL TTF done");
						//SDL_ShowCursor(SDL_DISABLE);
					}
				}
			}
		}
	}
	printf("  [%d]Initializing done with %d\n",SDL_GetTicks(), success);
	return success;
}

SDL_Texture* load_Texture(const char* path)
{
	SDL_Texture* newTexture = NULL;
	SDL_Surface* loadedSurf = IMG_Load(path);
	if(loadedSurf==NULL)
	{
		printf("Err load tex %s\n", IMG_GetError());
	}
	else
	{
		SDL_SetColorKey(loadedSurf, SDL_TRUE, SDL_MapRGB(loadedSurf->format,215,253,218));
		newTexture = SDL_CreateTextureFromSurface(rend, loadedSurf);
		if(newTexture == NULL)
		{
			printf("Err cr tex %s\n", IMG_GetError());
		}
		SDL_FreeSurface(loadedSurf);
	}
	return newTexture;
}

void LoadButtonsTex()
{
	LogoTex = load_Texture("files/logo.png");
	ButtonsTex = load_Texture("files/buttons.png");
	MainBackgr = load_Texture("files/mainback.png");
}

int load_font()
{
	int success=2;
	Fonts[0] = TTF_OpenFont("LiberationSans-Regular.ttf",24);
	if(Fonts[0] == NULL)
	{
		printf("Font loading error %s\n",TTF_GetError());
		success=0;
	}
	else
	{
		Fonts[1] = TTF_OpenFont("LiberationSans-Italic.ttf",24);
		if(Fonts[1] == NULL)
		{
			printf("Italic font loading error %s\n",TTF_GetError());
			success=1;
		}
		Fonts[2] = TTF_OpenFont("LiberationSans-BoldItalic.ttf",24);
		if(Fonts[2] == NULL)
		{
			printf("Font loading error %s\n",TTF_GetError());
			success=1;
		}
		Fonts[3] = TTF_OpenFont("LiberationSans-Bold.ttf",24);
		if(Fonts[3] == NULL)
		{
			printf("Font loading error %s\n",TTF_GetError());
			success=1;
		}
	}
	return success;
}

int CheckMouseButtonOverButtonInNewLaunch(int x, int y)
{
	if(x>=194 && x<=294 && y>=71 && y<=93)
	{
		return 0;
	}
	else
		return -1;
}

int NewSimulation()
{
	Uint32 windflg;
	windflg = SDL_WINDOW_SHOWN;
	newlaunchwind = SDL_CreateWindow("Orbita client v0.0.0.0.1pre-alpha test build 7", 
									SDL_WINDOWPOS_CENTERED, 
									SDL_WINDOWPOS_CENTERED, 
									600, 
									600, 
									windflg);
	newlaunchrend = SDL_CreateRenderer(newlaunchwind, -1, SDL_RENDERER_ACCELERATED);
	SDL_SetRenderDrawColor(newlaunchrend, 255, 255, 255, 255);
	SDL_RenderClear(newlaunchrend);
	
	SDL_Rect TextInputRect[10];
	SDL_Rect TextMassRect;
	
		
	SDL_Color ColorBlack;
	ColorBlack.r=0;
	ColorBlack.g=0;
	ColorBlack.b=0;
	ColorBlack.a=0;
	
	SDL_Surface* Text[20];
	int counter=0;
	for(counter=0; counter<20; counter++)
		Text[counter]=NULL;
	
	SDL_Rect TextRec[20];
	Text[0] = TTF_RenderUTF8_Blended(Fonts[3], "Настройки нового запуска", ColorBlack);
	TextRec[0].x=1;
	TextRec[0].y=1;
	TextRec[0].w=Text[0] -> w+20;
	TextRec[0].h=Text[0] -> h+4; //28
	Text[1] = TTF_RenderUTF8_Blended(Fonts[3], "Время запуска: (o) Сейчас", ColorBlack);
	TextRec[1].x=5;
	TextRec[1].y=38;
	TextRec[1].w=Text[1] -> w;
	TextRec[1].h=Text[1] -> h;
	Text[2] = TTF_RenderUTF8_Blended(Fonts[3], "Масса спутника:", ColorBlack);
	TextRec[2].x=5;
	TextRec[2].y=71;
	TextRec[2].w=Text[2] -> w;
	TextRec[2].h=Text[2] -> h;
	TextMassRect.x=10+Text[2] -> w;
	TextMassRect.y=71;
	TextMassRect.w=75;
	TextMassRect.h=28;
	Text[3] = TTF_RenderUTF8_Blended(Fonts[3], "Форма спутника: (o) Сфера", ColorBlack);
	TextRec[3].x=5;
	TextRec[3].y=104;
	TextRec[3].w=Text[3] -> w;
	TextRec[3].h=Text[3] -> h;
	Text[4] = TTF_RenderUTF8_Blended(Fonts[3], "(o) Сфера", ColorBlack);
	TextRec[4].x=Text[3] -> w +10;
	TextRec[4].y=104;
	TextRec[4].w=Text[3] -> w;
	TextRec[4].h=Text[3] -> h;
	Text[5] = TTF_RenderUTF8_Blended(Fonts[3], "Радиус спутника: (o) 1", ColorBlack);
	TextRec[5].x=5;
	TextRec[5].y=137;
	TextRec[5].w=Text[5] -> w;
	TextRec[5].h=Text[5] -> h;
	Text[6] = TTF_RenderUTF8_Blended(Fonts[3], "Время симуляции", ColorBlack);
	TextRec[6].x=5;
	TextRec[6].y=170;
	TextRec[6].w=Text[6] -> w;
	TextRec[6].h=Text[6] -> h;
	Text[7] = TTF_RenderUTF8_Blended(Fonts[3], "СТАРТ!", ColorBlack);
	Text[8] = TTF_RenderUTF8_Blended(Fonts[3], "Назад", ColorBlack);
	Text[9] = TTF_RenderUTF8_Blended(Fonts[3], "+", ColorBlack);
	Text[10] = TTF_RenderUTF8_Blended(Fonts[3], "-", ColorBlack);
	
	SDL_Rect MassPlusPos;
	MassPlusPos.x=10+(Text[2] -> w)+110;
	MassPlusPos.y=71;
	MassPlusPos.w=20;
	MassPlusPos.h=28;
	
	SDL_Rect MassMinusPos;
	MassMinusPos.x=10+(Text[2] -> w)+130;
	MassMinusPos.y=71;
	MassMinusPos.w=20;
	MassMinusPos.h=28;
	
	SDL_Rect StartButtonPos;
	StartButtonPos.x=SCREENSIZEY-100;
	StartButtonPos.y=SCREENSIZEY-30;
	StartButtonPos.w=Text[7] -> w;
	StartButtonPos.h=28;
	
	SDL_Texture* TextTex[20];
	for(counter=0; counter<20; counter++)
		TextTex[counter]=0;
	for(counter=0; counter<20; counter++)
		TextTex[counter] = SDL_CreateTextureFromSurface(newlaunchrend, Text[counter]);
	
	char textinput[300];
	bool exit=false;
	Uint32 startTime = SDL_GetTicks();
	SDL_Event evnt;
	int selection=0;
	int xmouse=0, ymouse=0;
	int MouseOnButtonNo=0;
	int numFrames = 0;
	Uint32 start;
	int mass=0;
	char masstext[20];
	
	while(!exit)
	{
		start = SDL_GetTicks();
		while(SDL_PollEvent(&evnt) != 0)
		{
			switch(evnt.type)
			{
				case SDL_QUIT:
				exit=true;
				prntdbg("SDL_QUIT event got");
				break;
				
				case SDL_MOUSEBUTTONDOWN:
				if(evnt.button.button == SDL_BUTTON_LEFT)
				{
					SDL_GetMouseState(&xmouse, &ymouse);
					MouseOnButtonNo = CheckMouseButtonOverButtonInNewLaunch(xmouse, ymouse);
					printf("  [%d]Mouse on x:%d y:%d; On button:%d\n", SDL_GetTicks(), xmouse, ymouse, MouseOnButtonNo);
					if(MouseOnButtonNo!=-1)
					{
						selection=MouseOnButtonNo;
					}
					switch(selection)
					{
						case -1:
						prntdbg("mouse miss");
						break;
						case 0:
						prntdbg("StartButton");
						break;
						case 1:
						prntdbg("BackButton");
						break;
						case 2:
						prntdbg("MassPlus");
						mass+=0.5;
						break;
						case 3:
						prntdbg("MassMinus");
						mass-=0.5;
						break;
						case 4:
						
						break;
						case 5:
						
						break;
					}
				}
				case SDL_TEXTINPUT:
				strcat(textinput, evnt.text.text);
				break;
				case SDL_TEXTEDITING:
				
				break;
			}
		}
		SDL_SetRenderDrawColor(newlaunchrend, 255, 255, 255, 255);
		SDL_RenderClear(newlaunchrend);
		SDL_SetRenderDrawColor(newlaunchrend, 0, 0, 0, 0);
		
		
		SDL_RenderCopy(newlaunchrend, TextTex[0], NULL, &TextRec[0]);
		SDL_RenderCopy(newlaunchrend, TextTex[1], NULL, &TextRec[1]);
		SDL_RenderDrawRect(newlaunchrend, &TextMassRect);
		sprintf(masstext, "%d        ", mass);
		SDL_RenderCopy(newlaunchrend, SDL_CreateTextureFromSurface(newlaunchrend, TTF_RenderUTF8_Blended(Fonts[3], masstext, ColorBlack)), NULL, &TextMassRect);
		SDL_RenderCopy(newlaunchrend, TextTex[9], NULL, &MassPlusPos);
		SDL_RenderCopy(newlaunchrend, TextTex[10], NULL, &MassMinusPos);
		SDL_RenderCopy(newlaunchrend, TextTex[2], NULL, &TextRec[2]);
		SDL_RenderCopy(newlaunchrend, TextTex[3], NULL, &TextRec[3]);
		//SDL_RenderCopy(newlaunchrend, TextTex[4], NULL, &TextRec[4]);
		SDL_RenderCopy(newlaunchrend, TextTex[5], NULL, &TextRec[5]);
		//SDL_RenderCopy(newlaunchrend, TextTex[6], NULL, &TextRec[6]);
		//SDL_RenderCopy(newlaunchrend, TextTex[6], NULL, &TextRec[6]);
		//SDL_RenderCopy(newlaunchrend, TextTex[6], NULL, &TextRec[6]);
		
		if(1000/FPS > SDL_GetTicks()-start) {
					SDL_Delay(1000/FPS-(SDL_GetTicks()-start));
		}
		SDL_RenderPresent(newlaunchrend);
		numFrames++;
	}
	
	SDL_DestroyWindow(newlaunchwind);
	return 0;
}

int CheckMouseButtonOverButton(int x, int y)
{
	if(x>=80 && x<=395 && y>=233 && y<=270)
	{
		return 0;
	}
	else if(x>=120 && x<=360 && y>=353 && y<=376)
	{
		return 1;
	}
	else if(x>=177 && x<=306 && y>=394 && y<=423)
	{
		return 2;
	}
	else if(x>=166 && x<=320 && y>=437 && y<=468)
	{
		return 3;
	}
	else if(x>=185 && x<=293 && y>=484 && y<=513)
	{
		return 4;
	}
	else
		return -1;
}

int main(int argc, char *argv[])
{
	init();
	
	load_font();
	
	LoadButtonsTex();
	
	SDL_Rect ButtonsTexPos;
	ButtonsTexPos.x=SCREENSIZEX/2-770/4;
	ButtonsTexPos.y=SCREENSIZEY/2-785/4;
	ButtonsTexPos.w=770/2;
	ButtonsTexPos.h=785/2;
	bool exit=false;
	int FPS_current=0;
	int numFrames = 0;
	Uint32 startTime = SDL_GetTicks();
	char fps_string[20];
	SDL_Event evnt;
	int selection=0;
	int xmouse=0, ymouse=0;
	int MouseOnButtonNo=0;
	Uint32 start;
	
	prntdbg("Enjoy main menu!");
	while(!exit)
	{
		start = SDL_GetTicks();
		while(SDL_PollEvent(&evnt) != 0)
		{
			if(evnt.type == SDL_QUIT)
			{
				exit=true;
				prntdbg("SDL_QUIT event got");
			}
			else if( evnt.type == SDL_MOUSEBUTTONDOWN )
			{
				if(evnt.button.button == SDL_BUTTON_LEFT)
				{
					SDL_GetMouseState(&xmouse, &ymouse);
					MouseOnButtonNo = CheckMouseButtonOverButton(xmouse, ymouse);
					printf("  [%d]Mouse on x:%d y:%d; On button:%d\n", SDL_GetTicks(), xmouse, ymouse, MouseOnButtonNo);
					selection=MouseOnButtonNo;
					switch(selection)
					{
						case -1:
						prntdbg("mouse miss");
						break;
						
						case 0:
						prntdbg("Selection = New simulation");
						NewSimulation();
						prntdbg("Back to the main menu");
						break;
						
						case 1:
						prntdbg("Selection = 2");
						//MessageBox(NULL, "This feature is still not ready\nSorry for that\nWe will add this in next updates...", "Emm... sry", MB_ICONEXCLAMATION | MB_OK);
						break;
						
						case 2:
						prntdbg("Selection = 3");
						//MessageBox(NULL, "This feature is still not ready\nSorry for that\nWe will add this in next updates...", "Emm... sry", MB_ICONEXCLAMATION | MB_OK);
						break;
						
						case 3:
						prntdbg("Selection = 4");
						break;
						
						case 4:
						prntdbg("Selection = exit");
						exit=true;
						break;
						
						default:
						prntdbg("hmm");
						break;
					}
				}
			}
		}
		SDL_SetRenderDrawColor(rend, 255, 255, 255, 255);
		SDL_RenderClear(rend);
		
		
		SDL_RenderCopy(rend, MainBackgr, NULL, NULL);
		SDL_RenderCopy(rend, ButtonsTex, NULL, &ButtonsTexPos);
		
		
		if(1000/FPS > SDL_GetTicks()-start) {
					SDL_Delay(1000/FPS-(SDL_GetTicks()-start));
		}
		SDL_RenderPresent(rend);
		numFrames++;
	}
	prntdbg("Bye!");
	//MessageBox(NULL, "Exit", "Exit", MB_ICONEXCLAMATION | MB_OK);
	return 0;
}