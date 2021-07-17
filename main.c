#define _POSIX_C_SOURCE 200809L
#include <assert.h>
#include <errno.h>
#include <getopt.h>
#include <limits.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <wayland-client.h>

#include "kanshi.h"
#include "wlr-output-management-unstable-v1-client-protocol.h"

#define HEADS_MAX 64


static void
head_handle_name(void *data, struct zwlr_output_head_v1 *wlr_head,
		const char *name)
{
	printf("head name %s\n", name);
}

static void
head_handle_description(void *data, struct zwlr_output_head_v1 *wlr_head,
		const char *description)
{
	printf("head description %s\n", description);
}

static void
head_handle_physical_size(void *data, struct zwlr_output_head_v1 *wlr_head,
		int32_t width, int32_t height) {
	printf("head physical size %dx%d\n", width, height);
}
static void
head_handle_mode(void *data, struct zwlr_output_head_v1 *wlr_head,
		struct zwlr_output_mode_v1 *wlr_mode) {
}

static void
head_handle_enabled(void *data, struct zwlr_output_head_v1 *wlr_head,
		int32_t enabled) {
	printf("head enabled %s\n", enabled ? "yes" : "no");
}

static void
head_handle_position(void *data, struct zwlr_output_head_v1 *wlr_head,
		int32_t x, int32_t y)
{
	printf("head position +%d+%d\n", x, y);
}

static void
head_handle_transform(void *data, struct zwlr_output_head_v1 *wlr_head, int32_t transform)
{
}

static void
head_handle_current_mode(void *data, struct zwlr_output_head_v1 *wlr_head,
		struct zwlr_output_mode_v1 *wlr_mode) {
}

static void
head_handle_scale(void *data, struct zwlr_output_head_v1 *wlr_head, wl_fixed_t scale)
{
	printf("head scale %gx\n", wl_fixed_to_double(scale));
}

static void
head_handle_finished(void *data, struct zwlr_output_head_v1 *wlr_head)
{
	printf("head finished\n");
}

static const struct zwlr_output_head_v1_listener head_listener = {
	.name = head_handle_name,
	.description = head_handle_description,
	.physical_size = head_handle_physical_size,
	.mode = head_handle_mode,
	.enabled = head_handle_enabled,
	.current_mode = head_handle_current_mode,
	.position = head_handle_position,
	.transform = head_handle_transform,
	.scale = head_handle_scale,
	.finished = head_handle_finished,
};

static void
output_manager_handle_head(void *data, struct zwlr_output_manager_v1 *manager,
		struct zwlr_output_head_v1 *wlr_head)
{
	printf("head\n");
	zwlr_output_head_v1_add_listener(wlr_head, &head_listener, NULL);
}

static void
output_manager_handle_done(void *data, struct zwlr_output_manager_v1 *manager,
		uint32_t serial)
{
	printf("done\n");
}

static void
output_manager_handle_finished(void *data, struct zwlr_output_manager_v1 *manager)
{
	printf("finished\n");
}

static const struct zwlr_output_manager_v1_listener output_manager_listener = {
	.head = output_manager_handle_head,
	.done = output_manager_handle_done,
	.finished = output_manager_handle_finished,
};

static void
registry_handle_global(void *data, struct wl_registry *registry,
		uint32_t name, const char *interface, uint32_t version)
{
	struct kanshi_state *state = data;

	if (strcmp(interface, zwlr_output_manager_v1_interface.name) == 0) {
		state->output_manager = wl_registry_bind(registry, name,
			&zwlr_output_manager_v1_interface, 1);
		zwlr_output_manager_v1_add_listener(state->output_manager,
			&output_manager_listener, NULL);
	}
}

static void
registry_handle_global_remove(void *data, struct wl_registry *registry, uint32_t name)
{
}

static const struct wl_registry_listener registry_listener = {
	.global = registry_handle_global,
	.global_remove = registry_handle_global_remove,
};

static const char usage[] =
	"Usage: %s [options...]\n"
	"  -h, --help           Show help message and quit\n";

static const struct option long_options[] = {
	{"help", no_argument, 0, 'h'},
	{0},
};

int
main(int argc, char *argv[])
{
	int opt;
	while ((opt = getopt_long(argc, argv, "h", long_options, NULL)) != -1) {
		switch (opt) {
		case 'h':
			fprintf(stderr, usage, argv[0]);
			return EXIT_SUCCESS;
		default:
			fprintf(stderr, usage, argv[0]);
			return EXIT_FAILURE;
		}
	}

	struct wl_display *display = wl_display_connect(NULL);
	if (display == NULL) {
		fprintf(stderr, "failed to connect to display\n");
		return EXIT_FAILURE;
	}

	struct kanshi_state state = {
		.running = true,
	};
	wl_list_init(&state.heads);

	struct wl_registry *registry = wl_display_get_registry(display);
	wl_registry_add_listener(registry, &registry_listener, &state);
	wl_display_dispatch(display);
	wl_display_roundtrip(display);

	if (!state.output_manager) {
		fprintf(stderr, "compositor doesn't support wlr-output-management-unstable-v1\n");
		return EXIT_FAILURE;
	}

	while (state.running && wl_display_dispatch(display) != -1)
		;

	return EXIT_SUCCESS;
}
