function notildes = remove_tildes(str);

notildes = str;
notildes = strrep(notildes, 'Á', 'A');
notildes = strrep(notildes, 'É', 'E');
notildes = strrep(notildes, 'Í', 'I');
notildes = strrep(notildes, 'Ó', 'O');
notildes = strrep(notildes, 'Ú', 'U');
notildes = strrep(notildes, 'Ñ', 'N');
notildes = strrep(notildes, 'á', 'a');
notildes = strrep(notildes, 'é', 'e');
notildes = strrep(notildes, 'í', 'i');
notildes = strrep(notildes, 'ó', 'o');
notildes = strrep(notildes, 'ú', 'u');
notildes = strrep(notildes, 'ñ', 'n');