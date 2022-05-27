#include <iostream>
#include <vector>
using namespace std;

//全域變數
int available[3];
int allocation[5][3];
int max_need[5][3];
int need[5][3];
vector<int>safety_sequence;
bool finish[5];

//全部有的函數名稱
void print_all_resource();
bool safety();
bool banker(int, vector<int>&);
void recover_from_banker(int, vector<int>&);

void print_all_resource(){
    cout << "Availiable resources: " ;
    for(int i = 0 ; i < 3 ; i ++){
        cout << available[i] << " ";
    }
    cout << "\n";
    cout << "    Resources Allocation | Max Request | Remaining Needs: \n";
    for(int i = 0 ; i < 5; i++){
        cout << "Process #" << i << "       ";
        for(int j = 0 ; j < 3 ; j++){
            cout << allocation[i][j] << " ";
        }
        cout << "  |    ";
        // max request
        for(int j = 0 ; j < 3 ; j++){
            cout << max_need[i][j] << " ";
        }
        cout << "   |    ";
        // pretend remaining needs
        for(int j = 0 ; j < 3 ; j++){ 
            cout << need[i][j] << " ";
        }
        cout << endl;
    }
}
bool safety(){ // 看能不能找到一組safety sequence
    for(int i = 0 ; i < 5 ; i++){
        finish[i] = false ;
    }
    int work[3];
    for(int i = 0 ; i < 3 ; i++){
        work[i] = available[i];
    }
    int ctr = 5;
    // sequence當某一組成功 那就把他塞進去!!
    while(true){
        bool atLeastOne = 0; //若在此雙層迴圈沒有加一 就代表沒有資源能分配到
        for(int i = 0 ; i < 5 ; i++){
            if(!finish[i]){//finish是false
                bool flag = false; 
                for(int j = 0 ; j < 3 ; j++){
                    if(need[i][j] > work[j]){
                        flag = true; //不符合
                        break;
                    }
                }
                if(flag == false){ // 確定是safe
                    safety_sequence.push_back(i);
                    finish[i] = true;
                    ctr -= 1;
                    atLeastOne = true;
                    for(int k = 0 ; k < 3 ; k++){
                        work[k] += allocation[i][k];
                    }
                }
            }
        }
        if(!ctr)break; //找到5個true，結束任務
        if(!atLeastOne)return false;
    }
    cout << "Safe! Find safe sequence:";
    for(int i = 0 ; i < safety_sequence.size() ; i++){
        cout << safety_sequence[i] << " ";
    }
    cout << endl;
    safety_sequence.clear();
    return true;
}
bool banker(int process,vector<int>&request){
    for(int i = 0 ; i < 3 ; i++){
        if(request[i] > need[process][i]){
            cout << "No No! you are too greedy\n";
            return false;
        }
        if(request[i] > available[i]){
            cout <<"Request > available ! process must wait! \n";
            return false;
        }
    }
    cout << endl;
    // ~~~~~~~~~pretend~~~~~~~~~~~~~~~~~~~~
    for(int i = 0 ; i < 3 ; i ++){
        available[i] = available[i] - request[i];
    }
    for(int j = 0 ; j < 3 ; j++){
        allocation[process][j] = allocation[process][j] + request[j];
    }
    for(int j = 0 ; j < 3 ; j++){ 
        need[process][j] = need[process][j] - request[j];
    }
    cout << "Pretend to grant the request \n";
    print_all_resource();
    return true;
}

void recover_from_banker(int process,vector<int>&request){ 
    cout << "enter the recovery section\n";
    for(int i = 0 ; i < 3 ; i ++){
        available[i] = available[i] + request[i];
    }
    for(int j = 0 ; j < 3 ; j++){
        allocation[process][j] = allocation[process][j] - request[j];
    }   
    // pretend remaining needs
    for(int j = 0 ; j < 3 ; j++){ 
        need[process][j] = need[process][j] + request[j];
    }
    cout <<"recovery has completed\n";
}

int main(){
    int a,b,c;
    cout << "=== Banker's algorithm starts! \n";
    cout << "Availiable resources: ";
    cin >> available[0] >> available[1] >> available[2];
    cout << endl;
    cout << "    Resources Allocation | Max Request | Remaining Needs: \n";
    for(int i = 0 ; i < 5; i++){ // 固定五個process
        cout << "Process #" << i << "       ";
        for(int j = 0 ; j < 3 ; j++){
            cin >> allocation[i][j];
        }
        cout << "  |    ";
        for(int j = 0 ; j < 3 ; j++){
            cin >> max_need[i][j];
        }
        cout << "   |    " ;
        for(int j = 0 ; j < 3 ; j++){
            cout << max_need[i][j] - allocation[i][j] << " ";
            need[i][j] = max_need[i][j] - allocation[i][j];
        }
        cout << "\n";
    }
    cout << "== Safety algorithm starts!\n\n";

    bool is_Safe = safety();
    if(!is_Safe){
        return 0;//Exit!
    }
    cout << "Safe! Grant the request\n";
    while(true){
        char isContine;
        int process;
        vector<int>change_allocate(3,0);
        cout << "Continue the Banker algorithm ? (Yes = y or Y)\n";
        cin >> isContine;
        if(isContine != 'Y' && isContine != 'y'){
            cout << "Bye! End of Banker's algorithn! \n";
            break;
        }
        cout << "Request! Enter process no: ";
        cin >> process;
        cout <<endl;
        for(int i = 0 ; i < 3 ; i++){
            cout << "Enter request resource #" << i << " = ";
            cin >> change_allocate[i];
            cout << "\n\n";
        }
        if(banker(process,change_allocate)){ // banker algo過了 再做safety
            cout << "== Safety algorithm starts!\n\n";
            bool isSafe = safety();
            if(!isSafe){
                cout << "Unsafe. Do not grant! Restore the allocation state! \n";
                recover_from_banker(process, change_allocate);
            }
            else{
                cout << "corret! you pass the banker's algo \n";
            }
        }
    }
    return 0;
}