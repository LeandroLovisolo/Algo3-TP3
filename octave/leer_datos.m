% El parámetro metrica puede valer 'tiempo' o 'frontera'
function data = leer_datos(ruta, metrica)

if(strcmp(metrica, 'tiempo'))
	columna = 2;
else
	columna = 3;
end

data = csvread(ruta);
data = data(:, [1 columna]);