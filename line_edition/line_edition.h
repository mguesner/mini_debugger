#ifndef LINE_EDITION_H
# define LINE_EDITION_H

typedef struct
{
	char			*line;
	int				err;
}					t_edit_line;

t_edit_line		*line_editor_init();
void			line_edition_pause(t_edit_line *line);
void			line_edition_cont(t_edit_line *line);
void			line_editor_end(t_edit_line *line);
int				line_edition_get_line(t_edit_line *line);
int				line_edition_getsubline(t_edit_line *line, char *comment);

#endif