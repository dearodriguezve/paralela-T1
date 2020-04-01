echo Ejecuciones
for k in 720p.jpg 1080.jpg 4k.jpg
do
echo "Imagen" $k
echo "Imagen" $k >> results.txt
for i in 1 2 4 8 16
do
echo $i hilos
echo $i hilos >> results.txt
for j in $(seq 3 15)
do
./output1 $k $j$i$k $j $i > results.txt
done
done
done
