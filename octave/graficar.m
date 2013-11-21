function graficar(algoritmos, datos, titulo, s)

colores = {'red', 'green', 'blue'};

figure;
hold on;

leyenda = [];

for i = 1:columns(datos)
	plot(datos{i}(:,1), datos{i}(:,2), 'color', colores{i});
	leyenda = [leyenda; algoritmos{i}];
end

xlim([datos{1}(1, 1) datos{1}(end, 1)]);

legend(leyenda);
legend('boxon');
legend('location', 'northwest');

xlabel('$n$');

if(strcmp(s.eje_y, 'frontera'))
	ylabel('Tamaño de la frontera de la CMF hallada');
else
	ylabel('Tiempo (milisegundos)');
end

title(titulo);