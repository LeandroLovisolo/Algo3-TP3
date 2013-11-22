function escaped = latex_escape(str)

escaped = str;
escaped = strrep(escaped, 'Á', '\''A');
escaped = strrep(escaped, 'É', '\''E');
escaped = strrep(escaped, 'Í', '\''I');
escaped = strrep(escaped, 'Ó', '\''O');
escaped = strrep(escaped, 'Ú', '\''U');
escaped = strrep(escaped, 'Ñ', '\~N');
escaped = strrep(escaped, 'á', '\''a');
escaped = strrep(escaped, 'é', '\''e');
escaped = strrep(escaped, 'í', '\''i');
escaped = strrep(escaped, 'ó', '\''o');
escaped = strrep(escaped, 'ú', '\''u');
escaped = strrep(escaped, 'ñ', '\~n');