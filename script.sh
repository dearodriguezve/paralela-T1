echo Ejecuciones
for k in 720p.jpg 1080p.jpg 4k.jpg
do
echo "Imagen" $k
echo "Imagen" $k >> results.txt
for i in 1 2 4 8 16
do
echo $i hilos
echo $i hilos >> results.txt
for j in 3 5 7 9 11 13 15
do
for x in $(seq 0 10)
do
./output $k "kr"$j"hi"$i"im"$k $j $i >> results.txt
done
echo "" >> results.txt
done
done
done
