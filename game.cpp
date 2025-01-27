
#pragma once
#include "game.h"
#include "general.h"
#include "color.h"
#include <filesystem>
#include <sstream>
#define SSTR( x ) dynamic_cast< std::ostringstream & >( \
            ( std::ostringstream() << std::dec << x ) ).str()

Game::Game() {
	INIT_R;
	SDL_Init(SDL_INIT_EVERYTHING);
	window = SDL_CreateWindow(
		"Cock Flock",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		WINDOW_W, WINDOW_H,
		SDL_WINDOW_HIDDEN
	);
	SDL_SetWindowIcon(window, IMG_Load("icon.png"));
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG) return;
	if (Mix_Init(MIX_INIT_OGG) != MIX_INIT_OGG) return;
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024) == -1) return;
	keyboard = SDL_GetKeyboardState(NULL);
	SDL_RenderSetScale(renderer, 1, 1);
	//for (const auto& entry : std::filesystem::directory_iterator("./sprites")) std::cout << entry.path() << std::endl;
	isOpen = load();
	assignImg();
	init();
	SDL_ShowWindow(window);
	SDL_GetMouseState(&mouse->x, &mouse->y);
	//SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
}

void Game::assignImg() {
	horda.img = images.get("horda");
	camera.img = images.get("mapa3");
	botonBack.img = images.get("back");
	botonShop.img = images.get("tienda");
	botonHardcore.img = images.get("hardcore");
	botonCreditos.img = images.get("creditosBoton");
	creditos.img = images.get("creditos");
}

void Game::init() {
	botonPlay.img = images.get("play");
	paredHitboxLeft = Cuadrado();
	paredHitboxRight = Cuadrado();
	paredHitboxLeft.dstRect = new SDL_Rect({ 1, 1, 150, 8100 });
	paredHitboxRight.dstRect = new SDL_Rect({ 810, 1, 150, 8100 });
	//rioHitboxLeft = Cuadrado();
	//rioHitboxLeft.dstRect = new SDL_Rect({ 150, -300, 210, 300 });
	//rioHitboxRight = Cuadrado();
	//rioHitboxRight.dstRect = new SDL_Rect({ 600, -300, 210, 300 });
	botonSonido.img = images.get("soundOn");
	botonSonido.dstRect = new SDL_Rect({ 10, 20, 100, 100 });
	botonPlay.dstRect = new SDL_Rect({ 10, 135, 100, 100 });
	botonBack.dstRect = new SDL_Rect({ 10, 250, 100, 100 });
	botonShop.dstRect = new SDL_Rect({ 10, 135, 100, 100 });
	botonExitShop.dstRect = new SDL_Rect({ 335, 650, 305, 65 });
	botonCompraAzul.dstRect = new SDL_Rect({ 535, 320, 130, 40 });
	botonCompraGolden.dstRect = new SDL_Rect({ 535, 540, 130, 40 });
	botonCompraDark.dstRect = new SDL_Rect({ 320, 540, 130, 40 });
	botonCompraBrown.dstRect = new SDL_Rect({ 320, 320, 130, 40 });
	botonHardcore.dstRect = new SDL_Rect({ (WINDOW_W / 2) - 410, 750, 330, 100 });
	botonCreditos.dstRect = new SDL_Rect({ WINDOW_W - 410, 750, 330, 100 });
	creditos.dstRect = new SDL_Rect({ 75, (WINDOW_H * 15 / 10), WINDOW_W - 150, WINDOW_H * 16 / 10 });
	creditos.sY = 2;
	nivel.dstRect = new SDL_Rect({ 0, 0, WINDOW_W, 0 });
	player.dstRect = new SDL_Rect({ (WINDOW_W / 2) - 42, WINDOW_H - 300 , 50, 50 });
	SDL_QueryTexture(images.get("mapa3"), NULL, NULL, NULL, &nivel.dstRect->h);
	camera.srcRect = new SDL_Rect({ 0, nivel.dstRect->h - WINDOW_H, WINDOW_W, WINDOW_H });
	horda.dstRect = new SDL_Rect({ 130, WINDOW_H, 0, 0 });
	continuara.img = images.get("continuara");
	continuara.sY = 6;
	pajaro.init(images.get("pajaro"));
	pajaro.dstRect = new SDL_Rect({ -200, R_NUM(0, WINDOW_H - 200), 70, 100 });
	pajaro.sX = 8;
	pajaro.sY = R_NUM(-4, 4);
	avestruz.sY = 1;
	continuara.dstRect = new SDL_Rect({ WINDOW_H / 2 - 325, (WINDOW_H * 15 / 10), 700 , 450 });
	avestruz.init(images.get("avestruz"));
	mascota.init(images.get("mascota"));
	mascota.dstRect = new SDL_Rect({ (WINDOW_W / 2) + 150, 75, 200, 200 });
	avestruz.dstRect = new SDL_Rect({ WINDOW_H / 2 - 100, (WINDOW_H * 15 / 10), 200, 200 });
	player.init(images.get("link"));
	SDL_QueryTexture(images.get("horda"), NULL, NULL, &horda.dstRect->w, &horda.dstRect->h);
	horda.dstRect->y = WINDOW_H - horda.dstRect->h;
	player.vides = 3;
	for (int i = 0; i < 3; i++) {
		player.corazones[i].img = images.get("corazon");
		player.corazones[i].alive = player.corazones[i].img;
		player.corazones[i].dead = images.get("corazont");
	}
}

bool Game::load() {
	//sfxs
	if (!sfxs.load("dañoGallina", "DanoContraGallina.wav")) return false;
	if (!sfxs.load("dañoQueja", "Danoqueja.wav")) return false;
	if (!sfxs.load("muerteGallina", "muertegallinaex.wav")) return false;
	if (!sfxs.load("disparoFlecha", "disparoflecha.wav")) return false;
	if (!sfxs.load("SMoneda", "Sonidomoneda.wav")) return false;
	if (!sfxs.load("MultitudG", "multitudG.wav")) return false;
	if (!sfxs.load("SStart", "sonidostart.wav")) return false;
	if (!tracks.load("Creditos", "Creditos.ogg")) return false;
	if (!tracks.load("Victoria", "VICTORIA.ogg")) return false;
	if (!tracks.load("Intro", "Intro Colibri Studios.ogg")) return false;
	if (!tracks.load("Game Over", "Game Over.ogg")) return false;
	if (!tracks.load("Gameplay", "Gameplay.ogg")) return false;
	if (!tracks.load("Menu", "Menu.ogg")) return false;
	if (!tracks.load("Tienda", "Tienda.ogg")) return false;
	if (!tracks.load("sonido de start", "sonidostart.ogg")) return false;
	//if (!tracks.load("Multidud de gallinas", "Multidud de gallinas.ogg")) return false;
	if (!images.load("soundOn", "soundOn.png")) return false;
	if (!images.load("soundOff", "soundOff.png")) return false;
	if (!images.load("mapa3", "mapa3.png")) return false;
	if (!images.load("play", "play.png")) return false;
	if (!images.load("continuara", "continuara.png")) return false;
	if (!images.load("soldOut", "soldOut.png")) return false;
	if (!images.load("creditos", "creditos.png")) return false;
	if (!images.load("play", "play.png")) return false;
	if (!images.load("link", "link.png")) return false;
	if (!images.load("rupia1", "rupia1.png")) return false;
	if (!images.load("rupia2", "rupia2.png")) return false;
	if (!images.load("rupia3", "rupia3.png")) return false;
	if (!images.load("rupia4", "rupia4.png")) return false;
	if (!images.load("pajaro", "pajaro.png")) return false;
	if (!images.load("mascota", "mascota.png")) return false;
	if (!images.load("corazon", "corazon.png")) return false;
	if (!images.load("corazont", "corazont.png")) return false;
	if (!images.load("enter", "press_enter.png")) return false;
	if (!images.load("pause", "pause.png")) return false;
	if (!images.load("studio", "studio.png")) return false;
	if (!images.load("horda", "horda.png")) return false;
	if (!images.load("flecha", "flecha.png")) return false;
	if (!images.load("flechab", "flecha_b.png")) return false;
	if (!images.load("start", "start.png")) return false;
	if (!images.load("back", "back.png")) return false;
	if (!images.load("gameoverT", "gameoverT.png")) return false;
	if (!images.load("linksad", "linksad.png")) return false;
	if (!images.load("popupTienda", "popupTienda.png")) return false;
	if (!images.load("arbol1", "arbol1.png")) return false;
	if (!images.load("tienda", "tienda.png")) return false;
	if (!images.load("roca4", "roca4.png")) return false;
	if (!images.load("roca1", "roca1.png")) return false;
	if (!images.load("roca2", "roca2.png")) return false;
	if (!images.load("roca3", "roca3.png")) return false;
	if (!images.load("lore1", "lore1.png")) return false;
	if (!images.load("lore2", "lore2.png")) return false;
	if (!images.load("lore3", "lore3.png")) return false;
	if (!images.load("lore4", "lore4.png")) return false;
	if (!images.load("lore5", "lore5.png")) return false;
	if (!images.load("lore6", "lore6.png")) return false;
	if (!images.load("lore7", "lore7.png")) return false;
	if (!images.load("lore8", "lore8.png")) return false;
	if (!images.load("lore9", "lore9.png")) return false;
	if (!images.load("lore10", "lore10.png")) return false;
	if (!images.load("lore11", "lore11.png")) return false;
	if (!images.load("lore12", "lore12.png")) return false;
	if (!images.load("lore13", "lore13.png")) return false;
	if (!images.load("tiendalore1", "Tiendalore_1.png")) return false;
	if (!images.load("tiendalore2", "Tiendalore_2.png")) return false;
	if (!images.load("tiendalore3", "Tiendalore_3.png")) return false;
	if (!images.load("tiendalore4", "Tiendalore_4.png")) return false;
	if (!images.load("tiendalore5", "Tiendalore_5.png")) return false;
	if (!images.load("tiendalore6", "Tiendalore_6.png")) return false;
	if (!images.load("tiendalore7", "Tiendalore_7.png")) return false;
	if (!images.load("tiendalore8", "Tiendalore_8.png")) return false;
	if (!images.load("tiendalore9", "Tiendalore_9.png")) return false;
	if (!images.load("n0", "0.png")) return false;
	if (!images.load("n1", "1.png")) return false;
	if (!images.load("n2", "2.png")) return false;
	if (!images.load("n3", "3.png")) return false;
	if (!images.load("n4", "4.png")) return false;
	if (!images.load("n5", "5.png")) return false;
	if (!images.load("n6", "6.png")) return false;
	if (!images.load("n7", "7.png")) return false;
	if (!images.load("n8", "8.png")) return false;
	if (!images.load("n9", "9.png")) return false;
	if (!images.load("arbol2", "arbol2.png")) return false;
	if (!images.load("arbol3", "arbol3.png")) return false;
	if (!images.load("arbol4", "arbol4.png")) return false;
	if (!images.load("pausaT", "pausaT.png")) return false;
	if (!images.load("gallina1", "gallinaBlanca.png")) return false;
	if (!images.load("gallina2", "gallinaMarron.png")) return false;
	if (!images.load("gallina3", "gallinaAzul.png")) return false;
	if (!images.load("gallina4", "gallinaOscura.png")) return false;
	if (!images.load("gallina5", "gallinaGolden.png")) return false;
	if (!images.load("hardcore", "hardcore.png")) return false;
	if (!images.load("winner", "winner.png")) return false;
	if (!images.load("tituloCockFlock", "tituloCockFlock.png")) return false;
	if (!images.load("creditosBoton", "creditosBoton.png")) return false;
	if (!images.load("avestruz", "avestruz.png")) return false;

	Mix_PlayMusic(tracks.get("Intro"), 1);
	return true;
}

Game::~Game() {
	images.clear();
	tracks.clear();
	sfxs.clear();
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void Game::input() {
	while (SDL_PollEvent(&event) != 0) {
		switch (event.type) {
		case SDL_QUIT:
			isOpen = false;
			break;
		case SDL_KEYDOWN:
			if (!event.key.repeat) {
				if (event.key.keysym.sym == SDLK_q && (escena == PAUSA || escena == TIENDA)) cambiaEscena(MENU);
				if ((
					event.key.keysym.sym == SDLK_RETURN ||
					event.key.keysym.sym == SDLK_SPACE
					) && escena == LORE) cambiaEscena(JOC);
				if ((
					event.key.keysym.sym == SDLK_RETURN ||
					event.key.keysym.sym == SDLK_SPACE
					) && escena == MENU) cambiaEscena(LORE);
				if ((
					event.key.keysym.sym == SDLK_RETURN ||
					event.key.keysym.sym == SDLK_SPACE
					) && escena == GAMEOVER) cambiaEscena(MENU);
				if ((
					event.key.keysym.sym == SDLK_RETURN ||
					event.key.keysym.sym == SDLK_SPACE
					) && escena == INICI) cambiaEscena(MENU);;
				if ((
					event.key.keysym.sym == SDLK_RETURN ||
					event.key.keysym.sym == SDLK_SPACE
					) && escena == GUANYAT) cambiaEscena(CREDITS);
				if (event.key.keysym.sym == SDLK_t && escena == MENU) cambiaEscena(TIENDA);
				if (event.key.keysym.sym == SDLK_F1) god = !god;
				if (event.key.keysym.sym == SDLK_m) {
					mute();
				}
				if (event.key.keysym.sym == SDLK_p) {
					pause();
				}
				if (escena == JOC && event.key.keysym.sym == SDLK_SPACE && (player.direccion == 1 || player.direccion == 3)) {
					Flecha* flecha = new Flecha();
					flecha->img = images.get("flecha");
					if (!muted) Mix_PlayChannel(-1, sfxs.get("disparoFlecha"), 0);
					if (player.direccion == 1) {
						flecha->sX = 0;
						flecha->sY = -5;
					}
					if (player.direccion == 3) {
						flecha->img = images.get("flechab");
						flecha->sX = 0;
						flecha->sY = 6;
					}
					SDL_Rect* origin = new SDL_Rect({ player.dstRect->x + 15, player.direccion == 3 ? (player.dstRect->y + player.dstRect->h + 5) : (player.dstRect->y - 5) , 30, 60 });
					flecha->dstRect = origin;
					flechas.push_back(flecha);
				}
			}
			break;
		case SDL_WINDOWEVENT:
			if (event.window.event == SDL_WINDOWEVENT_ENTER) {
				paused = false;
				//SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
			}
			if (event.window.event == SDL_WINDOWEVENT_LEAVE) paused = true;
			break;
		case SDL_MOUSEMOTION:
			SDL_GetMouseState(&mouse->x, &mouse->y);
			break;
		case SDL_MOUSEBUTTONUP:
			isClicking = true;
			break;
		}
	}
}

void Game::pause() {
	if (escena == JOC || escena == PAUSA) {
		paused = !paused;
		cambiaEscena(paused ? PAUSA : JOC);
		botonPlay.img = images.get(paused ? "pause" : "play");
		if (!muted) paused ? Mix_PauseMusic() : Mix_ResumeMusic();
	}
}

void Game::mute() {
	muted = !muted;
	botonSonido.img = images.get(muted ? "soundOff" : "soundOn");
	muted ? Mix_PauseMusic() : Mix_ResumeMusic();
}

void Game::cambiaEscena(Escena nuevaEscena) {
	switch (escena) {
	case INICI:
		break;
	case MENU:
		init();
		break;
	case LORE:
		break;
	case JOC:
		if (nuevaEscena != PAUSA) {
			for (Gallina* g : gallinas) g->disposable = true;
			for (Rupia* r : rupias) r->disposable = true;
			for (Flecha* f : flechas) f->disposable = true;
			for (Roca* r : rocas) r->disposable = true;
			for (Arbol* a : arboles) a->disposable = true;
		}
		break;
	case GAMEOVER:
		hardMode = false;
		init();
		break;
	case GUANYAT:
		hardMode = false;
		init();
		break;
	case TIENDA:
		init();
		break;
	case PAUSA:
		if (nuevaEscena != JOC) {
			for (Gallina* g : gallinas) g->disposable = true;
			for (Rupia* r : rupias) r->disposable = true;
			for (Flecha* f : flechas) f->disposable = true;
			for (Roca* r : rocas) r->disposable = true;
			for (Arbol* a : arboles) a->disposable = true;
		}
		break;
	case CREDITS:
		hardMode = false;
		break;
	default:
		break;
	}
	if (escena == MENU && nuevaEscena == LORE) partidesJugades++;
	//std::cout << partidesJugades << std::endl;
	if (escena == PAUSA && nuevaEscena == MENU) hardMode = false;
	escena = nuevaEscena;
	switch (nuevaEscena) {
		if (escena != LORE && escena != PAUSA) while (Mix_PlayingMusic()) Mix_HaltMusic();
	case INICI:
		break;
	case MENU:
		Mix_HaltChannel(-1);
		Mix_PlayMusic(tracks.get("Menu"), -1);
		dineroTemporal = 0;
		break;
	case LORE:
		if (!muted) Mix_PlayChannel(-1, sfxs.get("SStart"), 0);
		if (++loreShown > 13) loreShown = 1;
		break;
	case JOC:
		Mix_PlayMusic(tracks.get("Gameplay"), -1);
		if (!muted) Mix_PlayChannel(-1, sfxs.get("MultitudG"), -1);
		paused = false;
		if (hardMode) {
			horda.dstRect->y = WINDOW_H - horda.dstRect->h * 3;
		}
		break;
	case GAMEOVER:
		Mix_PlayMusic(tracks.get("Game Over"), 1);
		dineroTemporal = 0;
		break;
	case GUANYAT:
		Mix_HaltChannel(-1);
		Mix_PlayMusic(tracks.get("Victoria"), 1);
		break;
	case TIENDA:
		botonBack.dstRect = new SDL_Rect({ 10, 135, 100, 100 });
		Mix_PlayMusic(tracks.get("Tienda"), -1);
		player.dstRect->y = WINDOW_H - 120;
		if (++loreTienda > 9) loreTienda = 1;
		break;
	case PAUSA:
		Mix_HaltChannel(-1);
		break;
	case CREDITS:
		Mix_HaltChannel(-1);
		Mix_PlayMusic(tracks.get("Creditos"), 1);
		break;
	}
}

void Game::update() {
	destroy();
	if (keyboard[SDL_SCANCODE_ESCAPE]) isOpen = false;
	if (isClicking) {
		if (botonSonido.isClicked(mouse)) mute();
		else if (escena == LORE) cambiaEscena(JOC);
		if (escena == JOC && partidesJugades % 2 == 0 && pajaro.checkCollision(mouse)) {
			pajaro.dstRect->h = 0;
			dineroTemporal += 5;
		}
		if (botonCreditos.isClicked(mouse) && escena == MENU) cambiaEscena(CREDITS);
		if (botonBack.isClicked(mouse) && (escena == PAUSA || escena == TIENDA)) cambiaEscena(MENU);
		else if (botonShop.isClicked(mouse) && escena == MENU) cambiaEscena(TIENDA);
		if (botonHardcore.isClicked(mouse) && escena == MENU && !hardMode) hardMode = !hardMode;
		if (escena == MENU &&
			!botonShop.isClicked(mouse) &&
			!botonSonido.isClicked(mouse) &&
			!botonHardcore.isClicked(mouse)
			) cambiaEscena(LORE);
		if (escena == GUANYAT) cambiaEscena(CREDITS);
		if (escena == GAMEOVER) cambiaEscena(MENU);
		if (botonPlay.isClicked(mouse)) pause();
		if (escena == TIENDA) {
			if (botonCompraBrown.isClicked(mouse)) if (player.gallinasDesbloqueadas < 5 && !player.brownComprada && player.money >= 30) {
				player.money -= 30;
				player.brownComprada = true;
				player.gallinasDesbloqueadas++;
				botonCompraBrown.img = images.get("soldOut");
			}
			if (botonExitShop.isClicked(mouse)) {
				cambiaEscena(MENU);
			}
			if (botonCompraAzul.isClicked(mouse)) if (player.gallinasDesbloqueadas < 5 && !player.azulComprada && player.money >= 70) {
				player.money -= 70;
				player.azulComprada = true;
				player.gallinasDesbloqueadas++;
				botonCompraAzul.img = images.get("soldOut");
			}
			if (botonCompraDark.isClicked(mouse)) if (player.gallinasDesbloqueadas < 5 && !player.darkComprada && player.money >= 100) {
				player.money -= 100;
				player.darkComprada = true;
				player.gallinasDesbloqueadas++;
				botonCompraDark.img = images.get("soldOut");
			}
			if (botonCompraGolden.isClicked(mouse)) if (player.gallinasDesbloqueadas < 5 && !player.goldenComprada && player.money >= 150) {
				player.money -= 150;
				player.goldenComprada = true;
				player.gallinasDesbloqueadas++;
				botonCompraGolden.img = images.get("soldOut");
			}
		}
		isClicking = false;
	}
	switch (escena) {
	case INICI:
		break;
	case MENU:
		break;
	case LORE:
		break;
	case JOC:
		//if (keyboard[SDL_SCANCODE_Y]) camera.update();
		//if (keyboard[SDL_SCANCODE_J]) camera.srcRect->y += camera.sY;
		if (player.vides <= 0) cambiaEscena(GAMEOVER);
		if (!paused) {
			if (camera.srcRect->y > 0) {
				camera.update();
			}
			else if (camera.sY != 0) {
				for (Gallina* g : gallinas) g->disposable = true;
				for (Rupia* r : rupias) r->disposable = true;
				horda.dstRect->h = 0;
				player.money += dineroTemporal;
				cambiaEscena(GUANYAT);
			}

			//if (!player.checkCollision(rioHitboxRight.dstRect) && !player.checkCollision(rioHitboxLeft.dstRect)) 
			if (true) {
				if (keyboard[SDL_SCANCODE_W]) {
					player.direccion = 1;
					player.update(0, -1);
				}
				if (keyboard[SDL_SCANCODE_S]) {
					player.direccion = 3;
					player.update(0, 1);
				}
				if (keyboard[SDL_SCANCODE_A]) {
					player.direccion = 0;
					if (!player.checkCollision(paredHitboxLeft.dstRect)) player.update(-1, 0);
				}
				if (keyboard[SDL_SCANCODE_D]) {
					player.direccion = 2;
					if (!player.checkCollision(paredHitboxRight.dstRect)) player.update(1, 0);
				}
				/*for (Cuadrado* o : obstaculos) {
					if(player.checkCollision(o->dstRect)){
						if (player.dstRect->x + player.dstRect->w > o->dstRect->x && player.dstRect->x < o->dstRect->x + o->dstRect->w) {
							if (player.direccion == 1) {
								//ARRIBA
								player.dY == 0;
								break;
							}
							if (player.direccion == 3) {
								//ABAJO
								player.dY == 0;
								player.dstRect->y -= camera.sY;
								break;
							}
						}
						if (player.dstRect->y > + player.dstRect->h > o->dstRect->y && player.dstRect->y < o->dstRect->y + o->dstRect->h) {
							if (player.direccion == 0) {
								//IZQUIERDA
								player.dX == 0;
								break;
							}
							if (player.direccion == 2) {
								//DERECHA
								player.dX == 0;
								break;
							}
						}
					}
				}*/
			}
			if (partidesJugades % 2 == 0) {
				pajaro.update();
				if (pajaro.dstRect->x > WINDOW_W) {
					pajaro.sX = -8;
					pajaro.sY = R_NUM(-4, 4);
					pajaro.dstRect->y = R_NUM(0, WINDOW_H - 200);
				}
				if (pajaro.dstRect->x < -pajaro.dstRect->w) {
					pajaro.sX = 8;
					pajaro.sY = R_NUM(-4, 4);
					pajaro.dstRect->y = R_NUM(0, WINDOW_H - 200);
				}
			}
			if (player.checkCollision(horda.dstRect)) {
				player.damage();
				player.dstRect->y -= horda.dstRect->h + 10;
				if (!muted) Mix_PlayChannel(-1, sfxs.get("dañoGallina"), 0);
			}
			if (camera.srcRect->y > 900) {
				if ((SDL_GetTicks() / 16) % 300 == 0) for (int i = 0; i < 2; i++) {
					Rupia* rupia = new Rupia();
					rupia->tipus = 1;
					rupia->valor = 1;
					rupia->img = images.get("rupia" + std::to_string(rupia->tipus));
					rupia->dstRect = new SDL_Rect({ R_NUM(paredHitboxLeft.dstRect->w, WINDOW_W - (paredHitboxRight.dstRect->w * 2)), R_NUM(-250, -50), 55, 55 });
					rupias.push_back(rupia);
				}
				if ((SDL_GetTicks() / 16) % 450 == 0) for (int i = 0; i < 1; i++) {
					Rupia* rupia = new Rupia();
					rupia->tipus = R_NUM(2, 4);
					rupia->valor = 2;
					rupia->img = images.get("rupia" + std::to_string(rupia->tipus));
					rupia->dstRect = new SDL_Rect({ R_NUM(paredHitboxLeft.dstRect->w, WINDOW_W - (paredHitboxRight.dstRect->w * 2)), R_NUM(-250, -50), 55, 55 });
					rupias.push_back(rupia);
				}
				if ((SDL_GetTicks() / 16) % 150 == 0) {
					Gallina* gallina = new Gallina();
					gallina->tipus = 1;
					gallina->dstRect = new SDL_Rect({ R_NUM(paredHitboxLeft.dstRect->w, WINDOW_W - (paredHitboxRight.dstRect->w * 2)), R_NUM(-250, -50), 40, 40 });
					gallina->init(images.get("gallina" + std::to_string(gallina->tipus)));
					gallinas.push_back(gallina);
					gallina = new Gallina();
					if (player.brownComprada) {
						gallina->tipus = 2;
						gallina->dstRect = new SDL_Rect({ R_NUM(paredHitboxLeft.dstRect->w, WINDOW_W - (paredHitboxRight.dstRect->w * 2)), R_NUM(-250, -50), 40, 40 });
						gallina->init(images.get("gallina" + std::to_string(gallina->tipus)));
						gallinas.push_back(gallina);
					}
					if (player.azulComprada) {
						gallina = new Gallina();
						gallina->tipus = 3;
						gallina->dstRect = new SDL_Rect({ R_NUM(paredHitboxLeft.dstRect->w, WINDOW_W - (paredHitboxRight.dstRect->w * 2)), R_NUM(-250, -50), 40, 40 });
						gallina->init(images.get("gallina" + std::to_string(gallina->tipus)));
						gallinas.push_back(gallina);
					}
					if (player.darkComprada) {
						gallina = new Gallina();
						gallina->tipus = 4;
						gallina->dstRect = new SDL_Rect({ R_NUM(paredHitboxLeft.dstRect->w, WINDOW_W - (paredHitboxRight.dstRect->w * 2)), R_NUM(-250, -50), 40, 40 });
						gallina->init(images.get("gallina" + std::to_string(gallina->tipus)));
						gallinas.push_back(gallina);
					}
					if (player.goldenComprada) {
						gallina = new Gallina();
						gallina->tipus = 5;
						gallina->dstRect = new SDL_Rect({ R_NUM(paredHitboxLeft.dstRect->w, WINDOW_W - (paredHitboxRight.dstRect->w * 2)), R_NUM(-250, -50), 40, 40 });
						gallina->init(images.get("gallina" + std::to_string(gallina->tipus)));
						gallinas.push_back(gallina);
					}
				}
				if ((SDL_GetTicks() / 16) % 100 == 0) {
					Gallina* gallina = new Gallina();
					tipoGallinaTrasera = R_NUM(1, 5);
					if (tipoGallinaTrasera == 5 && !player.goldenComprada) tipoGallinaTrasera = 4;
					if (tipoGallinaTrasera == 4 && !player.darkComprada) tipoGallinaTrasera = 3;
					if (tipoGallinaTrasera == 3 && !player.azulComprada) tipoGallinaTrasera = 2;
					if (tipoGallinaTrasera == 2 && !player.brownComprada) tipoGallinaTrasera = 1;
					gallina->tipus = tipoGallinaTrasera;
					gallina->dstRect = new SDL_Rect({ R_NUM(paredHitboxLeft.dstRect->w, WINDOW_W - (paredHitboxRight.dstRect->w * 2)), WINDOW_H + R_NUM(-150, -50), 40, 40 });
					gallina->init(images.get("gallina" + std::to_string(gallina->tipus)));
					gallina->sY = -5;
					gallinas.push_back(gallina);
				}
				if ((SDL_GetTicks() / 16) % 300 == 0) for (int i = 0; i <= R_NUM(0, 1); i++) {
					Arbol* arbol = new Arbol();
					arbol->sX = 0;
					arbol->sY = camera.sY;
					arbol->img = images.get("arbol" + std::to_string(R_NUM(1, 4)));
					arbol->dstRect = new SDL_Rect({ R_NUM(paredHitboxLeft.dstRect->w, WINDOW_W - (paredHitboxRight.dstRect->w * 2)), -150 * R_NUM(1, 3), 40, 40 });
					SDL_QueryTexture(arbol->img, NULL, NULL, &arbol->dstRect->w, &arbol->dstRect->h);
					arbol->dstRect->w *= (35 / 10);
					arbol->dstRect->h *= (35 / 10);
					arboles.push_back(arbol);
				}
				if ((SDL_GetTicks() / 16) % 250 == 0) for (int i = 0; i <= R_NUM(0, 1); i++) {
					Roca* roca = new Roca();
					roca->sX = 0;
					roca->sY = camera.sY;
					roca->img = images.get("roca" + std::to_string(R_NUM(1, 4)));
					roca->dstRect = new SDL_Rect({ R_NUM(paredHitboxLeft.dstRect->w, WINDOW_W - (paredHitboxRight.dstRect->w * 2)), R_NUM(-450, -150), 40, 40 });
					roca->dstRect->x *= (15 / 10);
					roca->dstRect->y *= (15 / 10);
					rocas.push_back(roca);
				}
			}
			for (Rupia* r : rupias) {
				r->update(0, 1);
				if (player.checkCollision(r->dstRect)) {
					r->disposable = true;
					if (!muted) Mix_PlayChannel(-1, sfxs.get("SMoneda"), 0);
					dineroTemporal += r->valor;
				}
			}
			for (Gallina* g : gallinas) {
				g->update();
				if (player.checkCollision(g->dstRect)) {
					g->disposable = true;
					if (!muted) Mix_PlayChannel(-1, sfxs.get("dañoGallina"), 0);
					if (!god) player.damage();
				}
			}
			for (Roca* r : rocas) {
				r->update();
				if (player.checkCollision(r->dstRect)) {
					if (!muted) Mix_PlayChannel(-1, sfxs.get("dañoQueja"), 0);
					if (!hardMode) cambiaEscena(GAMEOVER);
					dineroTemporal = 0;
				}
			}
			for (Arbol* a : arboles) {
				a->update();
				if (player.checkCollision(a->dstRect) && !hardMode) {
					if (!muted) Mix_PlayChannel(-1, sfxs.get("dañoQueja"), 0);
					if (!hardMode) cambiaEscena(GAMEOVER);
					dineroTemporal = 0;
				}
			}
			for (Flecha* f : flechas) {
				f->update();
				for (Gallina* g : gallinas) {
					if (f->checkCollision(g->dstRect)) {
						if (!muted) Mix_PlayChannel(-1, sfxs.get("muerteGallina"), 0);
						g->disposable = true;
						f->disposable = true;
						dineroTemporal += R_NUM(0, g->tipus * 2);
					}
				}
			}
		}
		break;
	case GAMEOVER:
		break;
	case GUANYAT:
		break;
	case TIENDA:
		break;
	case PAUSA:
		break;
	case CREDITS:
		if (!creditsShown) {
			creditos.update(0, -1);
			if (creditos.dstRect->y < -creditos.dstRect->h) {
				creditsShown = true;
			}
		}
		if (!ostrichShown && creditsShown) {
			avestruz.update(0, -1);
			if (avestruz.dstRect->y < -avestruz.dstRect->h) {
				ostrichShown = true;
			}
		}
		if (ostrichShown && creditsShown) {
			continuara.update(0, -1);
			if (continuara.dstRect->y < -continuara.dstRect->h) {
				creditsShown = false;
				ostrichShown = false;
				cambiaEscena(MENU);
			}
		}
		break;
	}
}

void Game::draw() {
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);
	int w, h;
	std::string num, s;
	switch (escena) {
	case INICI:
		SDL_QueryTexture(images.get("studio"), NULL, NULL, &w, &h);
		SDL_RenderCopy(renderer, images.get("studio"), NULL, new SDL_Rect({ (WINDOW_W / 2) - 220, 150, 440, 440 }));
		SDL_QueryTexture(images.get("enter"), NULL, NULL, &w, &h);
		SDL_RenderCopy(renderer, images.get("enter"), NULL, new SDL_Rect({ WINDOW_W - 55 - w / 2, 550, w / 2, h * 7 / 10 }));
		break;
	case MENU:
		camera.sY = 0;
		camera.draw();
		player.draw();
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 128);
		SDL_RenderFillRect(renderer, new SDL_Rect({ 0, 0, WINDOW_W, WINDOW_H }));
		botonSonido.draw();
		botonShop.draw();
		SDL_QueryTexture(images.get("start"), NULL, NULL, &w, &h);
		SDL_RenderCopy(renderer, images.get("start"), NULL, new SDL_Rect({ WINDOW_W - 100 - w / 3, (WINDOW_H / 2) - (h * 4 / 10) / 2, w * 1 / 3, h * 4 / 10 }));
		SDL_QueryTexture(images.get("tituloCockFlock"), NULL, NULL, &w, &h);
		SDL_RenderCopy(renderer, images.get("tituloCockFlock"), NULL, new SDL_Rect({ (WINDOW_W / 2) - 160, 50, w / 2, h / 2 }));
		botonCreditos.draw();
		if (!hardMode) botonHardcore.draw();
		if (player.gallinasDesbloqueadas == 5) {
			mascota.draw();
			if ((SDL_GetTicks() / 16) % 20 == 0) mascota.animateX();
			if ((SDL_GetTicks() / 16) % (100 * mascota.spritesheet.maxC) == 0 && !paused) mascota.animateY();
		}
		break;
	case LORE:
		camera.draw();
		player.draw();
		botonSonido.draw();
		for (int i = 0; i < 3; i++) player.corazones[i].draw();
		if (loreShown == 0) loreShown = 1;
		SDL_QueryTexture(images.get("lore" + std::to_string(loreShown)), NULL, NULL, &w, &h);
		SDL_RenderCopy(renderer, images.get("lore" + std::to_string(loreShown)), NULL, new SDL_Rect({ (WINDOW_W / 2) - (w * 7 / 5) / 2, WINDOW_H - h - 50, w * 7 / 5, h * 12 / 10 }));
		break;
	case JOC:
		camera.sY = 2;
		camera.draw();
		botonSonido.draw();
		botonPlay.draw();
		/*if (camera.srcRect->y <= 6046)
		{
			rioHitboxLeft.draw();
			rioHitboxLeft.update(0, 1);
			rioHitboxRight.draw();
			rioHitboxRight.update(0, 1);
		}
		if (camera.srcRect->y <= 6030)
		{
			rioHitboxLeft.sY = camera.sY;
			rioHitboxRight.sY = camera.sY;
		}*/
		if ((SDL_GetTicks() / 16) % 20 == 0 && !paused) player.animateX();
		if (!paused) player.animateY();
		for (Rupia* r : rupias) r->draw();
		for (Roca* r : rocas) r->draw();
		for (Arbol* a : arboles) a->draw();
		for (Gallina* g : gallinas) {
			g->draw();
			if ((SDL_GetTicks() / 16) % 20 == 0 && !paused) g->animateX();
			if ((SDL_GetTicks() / 16) % 200 * g->spritesheet.maxC == 0 && !paused) g->animateY();
		}
		if ((SDL_GetTicks() / 16) % 20 == 0 && !paused) pajaro.animateX();
		player.draw();
		paredHitboxLeft.draw();
		paredHitboxRight.draw();
		for (int i = 0; i < 3; i++) player.corazones[i].draw();
		SDL_RenderCopy(renderer, images.get("rupia1"), NULL, new SDL_Rect({ WINDOW_W - 60, 90, 40, 40 }));
		num = std::to_string(dineroTemporal);
		for (int i = num.length() - 1; i >= 0; i--) {
			s = "n";
			s.append(1, num[i]);
			SDL_RenderCopy(renderer, images.get(s), NULL, new SDL_Rect({ WINDOW_W - 80 - ((int)num.length() - i) * 30, 88, 35, 40 }));
		}
		horda.draw();
		if (hardMode) {
			for (int i = 1; i < 3; i++) {
				SDL_RenderCopy(renderer, images.get("horda"), NULL, new SDL_Rect({ horda.dstRect->x, WINDOW_H - horda.dstRect->h * i, horda.dstRect->w, horda.dstRect->h }));
			}
		}
		for (Flecha* f : flechas) f->draw();
		if (partidesJugades % 2 == 0) {
			pajaro.draw();
			if ((SDL_GetTicks() / 16) % 20 == 0) pajaro.animateX();
		}
		break;
	case GAMEOVER:
		camera.sY = 0;
		camera.draw();
		player.draw();
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 200);
		SDL_RenderFillRect(renderer, new SDL_Rect({ 0, 0, WINDOW_W, WINDOW_H }));
		SDL_QueryTexture(images.get("gameoverT"), NULL, NULL, &w, &h);
		SDL_RenderCopy(renderer, images.get("gameoverT"), NULL, new SDL_Rect({ WINDOW_W - 100 - w / 3, 100, w * 1 / 3, h * 4 / 10 }));
		SDL_RenderCopy(renderer, images.get("linksad"), NULL, new SDL_Rect({ (WINDOW_W / 2) - 200, (WINDOW_H / 2), 350, 350 }));
		break;
	case GUANYAT:
		camera.sY = 0;
		camera.draw();
		player.draw();
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 200);
		SDL_RenderFillRect(renderer, new SDL_Rect({ 0, 0, WINDOW_W, WINDOW_H }));
		SDL_QueryTexture(images.get("winner"), NULL, NULL, &w, &h);
		SDL_RenderCopy(renderer, images.get("winner"), NULL, new SDL_Rect({ WINDOW_W / 2 - w / 2, WINDOW_H / 2 - h / 2, w, h }));
		break;
	case TIENDA:
		camera.sY = 0;
		camera.draw();
		player.draw();
		SDL_QueryTexture(images.get("popupTienda"), NULL, NULL, &w, &h);
		SDL_RenderCopy(renderer, images.get("popupTienda"), NULL, new SDL_Rect({ WINDOW_W / 4 , 20, w - 100, h - 100 }));
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 32);
		SDL_RenderFillRect(renderer, new SDL_Rect({ 0, 0, WINDOW_W, WINDOW_H }));
		if (loreTienda == 0) loreTienda = 1;
		SDL_QueryTexture(images.get("tiendalore" + std::to_string(loreTienda)), NULL, NULL, &w, &h);
		SDL_RenderCopy(renderer, images.get("tiendalore" + std::to_string(loreTienda)), NULL, new SDL_Rect({ WINDOW_W - 280, 640, w , h }));
		botonExitShop.draw();
		botonCompraAzul.draw();
		botonCompraGolden.draw();
		botonCompraDark.draw();
		botonCompraBrown.draw();
		botonBack.draw();
		botonSonido.draw();
		SDL_RenderCopy(renderer, images.get("rupia1"), NULL, new SDL_Rect({ WINDOW_W - 60, 90, 40, 40 }));
		num = std::to_string(player.money);
		for (int i = num.length() - 1; i >= 0; i--) {
			s = "n";
			s.append(1, num[i]);
			SDL_RenderCopy(renderer, images.get(s), NULL, new SDL_Rect({ WINDOW_W - 80 - ((int)num.length() - i) * 30, 88, 35, 40 }));
		}
		break;
	case PAUSA:
		camera.draw();
		player.draw();
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 128);
		SDL_RenderFillRect(renderer, new SDL_Rect({ 0, 0, WINDOW_W, WINDOW_H }));
		botonSonido.draw();
		botonPlay.draw();
		botonBack.draw();
		for (int i = 0; i < 3; i++) player.corazones[i].draw();
		if ((SDL_GetTicks() / 16) % 40 == 0) showPausaText = !showPausaText;
		if (showPausaText) {
			SDL_QueryTexture(images.get("pausaT"), NULL, NULL, &w, &h);
			SDL_RenderCopy(renderer, images.get("pausaT"), NULL, new SDL_Rect({ (WINDOW_W / 2) - 320 , WINDOW_H / 2 - h * 2 / 10, w * 1 / 3, h * 4 / 10 }));
		}
		break;
	case CREDITS:
		creditos.draw();
		avestruz.draw();
		if ((SDL_GetTicks() / 16) % 20 == 0) avestruz.animateX();
		continuara.draw();
		break;
	}
	SDL_RenderPresent(renderer);
}

void Game::destroy() {
	//std::cout << "Arboles antes: " + arboles.size() << std::endl;
	arboles.erase(std::remove_if(arboles.begin(), arboles.end(), [](const Cuadrado* o) {
		bool temp = o->dstRect->x < -o->dstRect->w || o->dstRect->x > WINDOW_W + o->dstRect->w || o->dstRect->y > WINDOW_H + o->dstRect->h;
		return o->disposable || temp;
		}), arboles.end());
	//std::cout << "Arboles despues: " + arboles.size() << std::endl;
	//std::cout << "Rocas antes: " + rocas.size() << std::endl;
	rocas.erase(std::remove_if(rocas.begin(), rocas.end(), [](const Cuadrado* o) {
		bool temp = o->dstRect->x < -o->dstRect->w || o->dstRect->x > WINDOW_W + o->dstRect->w || o->dstRect->y > WINDOW_H + o->dstRect->h;
		return o->disposable || temp;
		}), rocas.end());
	//std::cout << "Rocas despues: " + rocas.size() << std::endl;
	//std::cout << "Rupias antes: " + rupias.size() << std::endl;
	rupias.erase(std::remove_if(rupias.begin(), rupias.end(), [](const Cuadrado* o) {
		bool temp = o->dstRect->x < -o->dstRect->w || o->dstRect->x > WINDOW_W + o->dstRect->w || o->dstRect->y > WINDOW_H + o->dstRect->h;
		return o->disposable || temp;
		}), rupias.end());
	//std::cout << "Rupias despues: " + rupias.size() << std::endl;
	//std::cout << "Gallinas antes: " + gallinas.size() << std::endl;
	gallinas.erase(std::remove_if(gallinas.begin(), gallinas.end(), [](const Cuadrado* o) {
		bool temp = o->dstRect->x < -o->dstRect->w || o->dstRect->x > WINDOW_W + o->dstRect->w || o->dstRect->y > WINDOW_H + o->dstRect->h;
		return o->disposable || temp;
		}), gallinas.end());
	//std::cout << "Gallinas despues: " + gallinas.size() << std::endl;
	//std::cout << "Flechas antes: " + flechas.size() << std::endl;
	flechas.erase(std::remove_if(flechas.begin(), flechas.end(), [](const Cuadrado* o) {
		bool temp = o->dstRect->x < -o->dstRect->w || o->dstRect->x > WINDOW_W + o->dstRect->w || o->dstRect->y < -o->dstRect->h || o->dstRect->y > WINDOW_H + o->dstRect->h;
		return o->disposable || temp;
		}), flechas.end());
	//std::cout << "Flechas despues: " + flechas.size() << std::endl;
}

void Game::loop() {
	input();
	update();
	draw();
	SDL_Delay(1000 / 60);
}
