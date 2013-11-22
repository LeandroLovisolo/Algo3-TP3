function guardar_como_tex(archivo_tex)

cd('tex');
print(archivo_tex, '-S950,450', '-dtex');
cd('..');
close();