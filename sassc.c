#include <stdio.h>
#include <getopt.h>
#include "libsass/sass_interface.h"

int main(int argc, char** argv)
{
	struct sass_file_context* ctx;
	int ret;
	int style = SASS_STYLE_NESTED; /* Default style */
	int Iflag = 0;
	char *Iflag_arg;
	char *filename;
	int c;

	while ((c = getopt(argc, argv, "cI:")) != -1) {
		switch (c) {
		case 'I':
			Iflag_arg = optarg;
			Iflag = 1;
			break;
		case 'c':
			style = SASS_STYLE_COMPRESSED;
			break;
		case '?':
			/* Unrecognized flag or missing an expected value */
			/* getopt should produce it's own error message for this case */
			return 1;
		default:
			fprintf(stderr, "Unknown error while processing arguments\n");
			return 2;
		}
	}

	if (optind < argc) {
		filename = argv[optind];
	} else {
		fprintf(stderr, "Usage: sassc [OPTION]... FILE\n");
		return 1;
	}

	ctx = sass_new_file_context();

	if (Iflag == 1) {
		ctx->options.include_paths = Iflag_arg;
	} else {
		ctx->options.include_paths = "";
	}

	ctx->options.image_path = "images";
	ctx->options.output_style = style;
	ctx->input_path = filename;

	sass_compile_file(ctx);

	if (ctx->error_status) {
		if (ctx->error_message)
			fprintf(stderr, "%s", ctx->error_message);
		else
			fprintf(stderr, "An error occured; no error message available.\n");
		ret = 1;
	}
	else if (ctx->output_string) {
		printf("%s", ctx->output_string);
		ret = 0;
	}
	else {
		fprintf(stderr, "Unknown internal error.\n");
		ret = 2;
	}

	sass_free_file_context(ctx);
	return ret;
}

