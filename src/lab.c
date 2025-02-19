/**Update this file with the starter code**/

char *get_prompt(const char *env) {
}

int change_dir(char **dir) {
}

char **cmd_parse(char const *line) {
}

void cmd_free(char ** line) {
}

char *trim_white(char *line) {
	// Trim the front
	int lineLength = strlen(line);
	int i = 0;
	while (i < lineLength && isspace(line[i])) {
		i++;
	}
	memmove(line, line + i, lineLength - i + 1);
	
	// Trim the back
	lineLength -= i;
	i = lineLength - 1;
	while ( i > 0 && isspace(line[i])) {
		line[i] = '\0';
		i--;
	}

	return line
}

bool do_builtin(struct shell *sh, char **argv){
}

void sh_init(struct shell *sh) {
}

void sh_destroy(struct shell *sh) {
}

void parse_args(int argc, char **argv) {
}
