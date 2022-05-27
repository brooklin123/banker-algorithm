#include <iostream>
#include <vector>
using namespace std;
int available[3];
int allocation[5][3];
int max_need[5][3];
int need[5][3];
vector<int>safety_sequence;
bool finish[5]; //初始都是false
//先把Safety會需要的印一印
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
    cout <<"~~~~~~~~~~~~check safety resource~~~~~~~~~~~~\n";
    print_all_resource();
    cout <<"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
    int work[3];
    for(int i = 0 ; i < 3 ; i++){
        work[i] = available[i];
    }
    int fin = 5;
    // sequence當某一組成功 那就把他塞進去!!
    while(true){
        bool canDoIt = 0; //若在此雙層迴圈沒有加一 就代表沒有資源能分配到
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
                    fin -= 1;
                    canDoIt = true;
                    cout << "work : ";
                    for(int k = 0 ; k < 3 ; k++){
                        work[k] += allocation[i][k];
                        cout << work[k] << " ";
                    }
                    cout <<" process : "<< i << endl;
                }
            }
        }
        if(!fin)break; //找到5個true，結束任務 // 記這層回圈 從網上加 
        if(!canDoIt)return false;
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
        if(request[i] > need[process][i] || request[i] > available[i]){
            return false;
        }
    }
    cout << endl;
    // ~~~~~~~~~列印pretend後的值~~~~~~~~~~~~~~~~~~~~~~ 
    cout << "Pretend to grant the request \n";
    cout << "Availiable resources: " ;
    for(int i = 0 ; i < 3 ; i ++){
        available[i] = available[i] - request[i];
        // cout << available[i] << " ";
    }
    cout << "\n";
    cout << "    Resources Allocation | Max Request | Remaining Needs: \n";
    for(int i = 0 ; i < 5; i++){
        cout << "Process #" << i << "       ";
        for(int j = 0 ; j < 3 ; j++){
            if(i == process){
                allocation[i][j] = allocation[i][j] + request[j];
            }
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
            if(i == process){
                need[i][j] = need[i][j] - request[j];
            } 
            cout << need[i][j] << " ";
        }
        cout << endl;
    }
    return true;
}
void recover_from_banker(int process,vector<int>&request){
        // ~~~~~~~~~列印pretend後的值~~~~~~~~~~~~~~~~~~~~~~ 
    cout << "Pretend to grant the request \n";
    cout << "Availiable resources: " ;
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
    //第一頁的""RESOURCE-REQUEST algrithm stats!"
    // ....
    cout << "Safe! Grant the request\n";
    //起董循環~
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
        for(int i = 0 ; i < 3 ; i++){
            cout << "Enter request resource #" << i << " = ";
            cin >> change_allocate[i];
            cout << "\n\n";
        }
        if(banker(process,change_allocate)){ // banker algo過了 再做safety
            cout << "== Safety algorithm starts!\n\n";
            bool isSafe = safety(); //??? 第二次發 process:1 quest : 102 是false???
            if(!isSafe){
                cout << "Unsafe. Do not grant! Restore the allocation state! \n";//unsafe 要recover
                recover_from_banker(process, change_allocate);
            }
            else{
                cout << "corret! you pass the banker's algo \n";
            }
        }
        else{
            cout << "No No! you are too greedy\n";
        }
    } // 當不繼續 那就可以跳出此while~
    return 0;
}