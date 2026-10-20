#include <iostream>
#include <fstream>
#include <string>
#include <vector>
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

void createFile(string filename, int n){
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

int main(){
    cout << "Podaj liczbę wierzchołków grafu: ";
    int n;
    cin >> n;

    createFile("input.txt", n);
    cout << "Plik utworzono pomyślnie.";

    return 0;
}
