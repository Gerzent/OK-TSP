#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <random>
using namespace std;

vector<string> split(string s, string delimeter){
    vector<string> splitted;
    int pos = s.find(delimeter);
    if(pos==-1){
        return {"error"};
    }
    string token = s.substr(0, pos);
    splitted.push_back(token);
    s.erase(0, pos+1);
    splitted.push_back(s);
    return splitted;
}

void createFileManual(string filename, int n){
    ofstream input(filename);
    input << n << endl;
    vector<string> visited;
    cout << "Podaj współrzędne x i y dla każdego wierzchołka, oddzielone przecinkiem." << endl;
    for(int i=1; i<=n; i++){
        bool proceed=false;
        while (!proceed){
            int error=0;
            cout << i << ": ";
            int x;
            int y;
            // input usera
            string xy; 
            cin >> xy;
            // rozdzielenie współrzędnych oraz
            // obsługa błędów spowodowanych złym imputem
            vector<string> xyv = split(xy, ",");
            if(xyv[0]!="error"){
                try{
                    x = stoi(xyv[0]);
                    y = stoi(xyv[1]);
                }
                catch(...){
                    error=1;
                }
            }
            else{
                error=1;
            }

            // sprawdzenie czy współrzędne się nie powtarzają
            if(error==0){
                for(int j=0; j<visited.size(); j++){
                    vector<string> v = split(visited[j], ",");
                    if(v[0]==to_string(x) && v[1]==to_string(y)){
                        error = 2;
                        break;
                    }
                }
            }

            // obsługa błędów
            if(error==0){
                input << i << " ";
                input << x << " ";
                input << y << endl;
                visited.push_back(xy);
                proceed = true;
            }
            else{
                switch(error){
                    case 1:
                        cout << "Niepoprawne współrzędne." << endl;
                        break;
                    case 2:
                        cout << "Podane współrzędne są już wykorzystywane przez inny wierzchołek." << endl;
                        break;
                    default:
                        cout << "Błąd nr " << error << endl;
                        break;
                }
            }
        }
    }

    input.close();
}

bool createFileRandom(string filename, vector<int> mRange, vector<int> xRange, vector<int> yRange){
    if(mRange[0]<1 || mRange[1]<1 || xRange[0]<1 || xRange[1]<1 || yRange[0]<1 || yRange[1]<1){
        return false;
    }
    else{
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<> distribN(mRange[0], mRange[1]);

        int n = distribN(gen);
        ofstream input(filename);
        input << n << endl;
        vector<string> visited;
        for(int i=1; i<=n; i++){
            bool repeat=true;
            int x;
            int y;
            string xy;
            while(repeat==true){
                repeat = false;
                random_device rdX;
                mt19937 genX(rdX());
                uniform_int_distribution<> distribX(xRange[0], xRange[1]);

                random_device rdY;
                mt19937 genY(rdY());
                uniform_int_distribution<> distribY(yRange[0], yRange[1]);

                x = distribX(genX);
                y = distribY(genY);
                xy = to_string(x) + "," + to_string(y);

                // sprawdzenie czy wierzchołki się nie powtarzają
                for(int j=0; j<visited.size(); j++){
                    vector<string> v = split(visited[j], ",");
                    if(v[0]==to_string(x) && v[1]==to_string(y)){
                        repeat = true;
                        break;
                    }
                }
            }
            input << i << " ";
            input << x << " ";
            input << y << endl;
            visited.push_back(xy);
        }
        return true;
    }
}

int main() {
    cout << "Jak chcesz wygenerować plik?" << endl;
    cout << "1. Ręcznie." << endl;
    cout << "2. Losowo." << endl;
    
    int choice;
    cin >> choice;

    if(choice == 1){
        int n;
        cout << "Podaj liczbę wierzchołków grafu: ";
        cin >> n;

        if(n <= 0){
            cout << "Liczba wierzchołków musi byc większa od 0." << endl;
            return 1;
        }

        createFileManual("input.txt", n);
        cout << "Plik zostal wygenerowany pomyślnie." << endl;

    } 
    else if(choice == 2){
        vector<int> nRange(2);
        vector<int> xRange(2);
        vector<int> yRange(2);

        cout << "Podaj minimalną liczbę wierzchołków: ";
        cin >> nRange[0];
        cout << "Podaj maksymalną liczbę wierzchołków: ";
        cin >> nRange[1];

        if(nRange[0] > nRange[1]){
            cout << "Wartość minimalna nie może być większa od maksymalnej." << endl;
            return 1;
        }

        cout << "Podaj minimalną wartość współrzędnej X: ";
        cin >> xRange[0];
        cout << "Podaj maksymalną wartość współrzędnej X: ";
        cin >> xRange[1];

        if(xRange[0] > xRange[1]){
            cout << "Wartość minimalna nie może być większa od maksymalnej." << endl;
            return 1;
        }

        cout << "Podaj minimalną wartość współrzędnej Y: ";
        cin >> yRange[0];
        cout << "Podaj maksymalną wartość współrzędnej Y: ";
        cin >> yRange[1];

        if(yRange[0] > yRange[1]){
            cout << "Wartość minimalna nie może być większa od maksymalnej." << endl;
            return 1;
        }

        bool status = createFileRandom("input.txt", nRange, xRange, yRange);
        if(status){
            cout << "Plik został wygenerowany pomyślnie." << endl;
        } 
        else{
            cout << "Błąd podanych parametrów. Wskazane wartości muszą być większe od 0." << endl;
        }

    }
    else{
        cout << "Niepoprawny wybór." << endl;
        return 1;
    }

    return 0;
}
