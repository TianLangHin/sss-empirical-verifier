if [ "$#" -eq 3 ]; then
  g++ -D WIDTH=$1 -D DEPTH=$2 -std=c++23 $(find . -name "*.cpp") -o main
  ./main $3
  rm main
elif [ "$#" -eq 2 ]; then
  g++ -D WIDTH=$1 -D DEPTH=$2 -std=c++23 $(find . -name "*.cpp") -o main
  ./main
  rm main
else
  echo "Please input width and depth first."
fi
