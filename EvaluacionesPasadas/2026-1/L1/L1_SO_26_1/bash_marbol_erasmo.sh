while true; do
    clear
    echo "=== PROCESS TREE ==="
    ps -l | head -1
    ps -l | grep '[m]arbol'
    sleep 0.2
done