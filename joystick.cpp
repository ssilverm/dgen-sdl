#ifdef WITH_JOYSTICK

#include <stddef.h>
#include <SDL.h>
#include <SDL_joystick.h>
#include "md.h"

static SDL_Joystick *(*handles)[];
static unsigned int handles_n;

void md::init_joysticks()
{
	int n;
	unsigned int i;
	SDL_Joystick *(*tmp)[];

	deinit_joysticks();
  // Initialize the joystick support
  // Thanks to Cameron Moore <cameron@unbeatenpath.net>
  if(SDL_InitSubSystem(SDL_INIT_JOYSTICK) < 0)
    {
      fprintf(stderr, "joystick: Unable to initialize joystick system\n");
      return;
    }
	n = SDL_NumJoysticks();
	if (n <= 0) {
		fprintf(stderr, "joystick: no joystick found\n");
		return;
	}
	fprintf(stderr, "joystick: %d joystick(s) found\n", n);
	tmp = (SDL_Joystick *(*)[])malloc(sizeof((*tmp)[0]) * n);
	if (tmp == NULL) {
		fprintf(stderr, "joystick: unable to allocate memory\n");
		return;
	}
	// Open all of them.
	for (i = 0; (i != (unsigned int)n); ++i) {
		SDL_Joystick *handle = SDL_JoystickOpen(i);

		if (handle == NULL)
			fprintf(stderr, "joystick: can't open joystick %u: %s",
				i, SDL_GetError());
		else
			fprintf(stderr,
				"joystick #%u:, %d %s, %d button(s),"
				" %d hat(s), name: \"%s\"\n",
				i,
				SDL_JoystickNumAxes(handle),
				((SDL_JoystickNumAxes(handle) == 1) ?
				 "axis" : "axes"),
				SDL_JoystickNumButtons(handle),
				SDL_JoystickNumHats(handle),
				SDL_JoystickName(i));
		(*tmp)[i] = handle;
	}
	handles = tmp;
	handles_n = i;
  // Enable joystick events
  SDL_JoystickEventState(SDL_ENABLE);
}

void md::deinit_joysticks()
{
	unsigned int n = handles_n;
	SDL_Joystick *(*tmp)[] = handles;

	handles_n = 0;
	handles = NULL;
	if (tmp == NULL)
		return;
	while (n != 0) {
		--n;
		if ((*tmp)[n] == NULL)
			continue;
		SDL_JoystickClose((*tmp)[n]);
		(*tmp)[n] = NULL;
	}
	free(tmp);
	SDL_QuitSubSystem(SDL_INIT_JOYSTICK);
}

#else // WITH_JOYSTICK

// Avoid empty translation unit.
typedef int md_phil;

#endif // WITH_JOYSTICK
