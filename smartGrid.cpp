#include <bits/stdc++.h>
using namespace std;

const int N = 20;//number of devices
int price[25]; // price[1] to price[24] -> price of 24 hours in a day
int pref[25]; //pref[i] = price[1] +..+ price[i]
int sol[N+1][25];
int discomfort;
int target;

struct appliance{
    //type - 0 -> uncontrollable
    //       1 -> controllable uninterruptable
    int type;
    int time_req;
    int start;
    int end;
    int units_per_hour;
};

appliance device[N+1];

int random_sol(int n, int this_day, int & discomfort){
    int ret = 0;
    int ans[n+1][25];
    int total_rand = 10000;
    int t1;
    srand(time(0));
    while(total_rand--){
        int cur_discomfort = 0;
        int cur_price = 0;
        for(int i=1;i<=n;++i){
            int s = device[i].start;
            int e = device[i].end;
            int req = device[i].time_req;
            int units = device[i].units_per_hour * (req);
            int type = device[i].type;
            if(type==0){
                // uncontrollable
                cur_price += units*(pref[s+req-1]-pref[s-1]);
                for(int j=1;j<=24;++j){
                    if(j>=s && j <= s+req-1){
                        ans[i][j] = 1;
                    }else{
                        ans[i][j] = 0;
                    }
                }
                continue;
            }
            if(type==1){
                int idx = s+rand()%(e-req+1);
                cur_discomfort += idx - s;
                cur_price += units*(pref[idx+req-1]-pref[idx-1]);
                for(int j=1;j<=24;++j){
                    if(j>=idx && j<=idx+req-1){
                        ans[i][j] = 1;
                    }else{
                        ans[i][j] = 0;
                    }
                }
            }
        }
        if(cur_price<=this_day && cur_discomfort<discomfort){
            ret = 1;
            discomfort = cur_discomfort;
            t1 = target - cur_price;
            for(int i=1;i<=n;++i){
                for(int j=1;j<=24;++j){
                    sol[i][j] = ans[i][j];
                }
            }
        }
    }
    if(ret){
        target = t1;
    }
    return ret;
}

int main(){
    ifstream cin;
    cin.open("input.txt");
    int days;
    cout << "Enter number of days \n";
    cin >> days;
    cout << "Enter target budget\n";
    cin >> target;
    int this_day;
    int n;
    cout << "Enter number of appliances \n";
    cin >> n;    
    for(int dd = days;dd>=1;--dd){
        this_day = target/dd;
        cout << "\tType\tTime required\tStart Time\tDeadline\tUnits per/hr\n";
        for(int i=1;i<=n;++i){
            cout << i<<">\t";
            cin >> device[i].type >> device[i].time_req >> device[i].start >> device[i].end >> device[i].units_per_hour;
        }
        cout << "Enter price for every hour (hour 1 to 24)\n";
        for(int i=1;i<=24;++i){
            cin >> price[i];
            pref[i] = pref[i-1] + price[i];
        }
    // checking for zero discomfort
    int cur_price = 0;
    for(int i=1;i<=n;++i){
        int s = device[i].start;
        int e = device[i].end;
        int req = device[i].time_req;
        int units = device[i].units_per_hour * (req);
        cur_price += units*(pref[s+req-1]-pref[s-1]);
    }
    if(cur_price<=this_day){
        for(int i=1;i<=n;++i){
            int s = device[i].start;
            int e = device[i].end;
            int req = device[i].time_req;
            int units = device[i].units_per_hour * (req);
            for(int j=1;j<=24;++j){
                if(j>=s && j <= s+req-1){
                    cout << "1 ";
                }else{
                    cout << "0 ";
                }
            }
            cout << endl;
        }
        target -= cur_price;
        continue;
    }
    // greedy algorithm for minimum cost
    cur_price = 0;
    for(int i=1;i<=n;++i){
        int s = device[i].start;
        int e = device[i].end;
        int req = device[i].time_req;
        int units = device[i].units_per_hour * (req);
        int type = device[i].type;
        
        if(type==0){
            // uncontrollable
            cur_price += units*(pref[s+req-1]-pref[s-1]);
            for(int j=1;j<=24;++j){
                if(j>=s && j <= s+req-1){
                    sol[i][j] = 1;
                }else{
                    sol[i][j] = 0;
                }
            }
            
            continue;
        }
        if(type==1){
            // controllable uninterruptable
            int minm = pref[s+req-1] - pref[s-1];
            int ga = s, gb = s+req-1;
            for(int a = s; a<=e;++a){
                int b = a+req-1;
                
                if(b>e){
                    break;
                }
                if(pref[b]-pref[a-1]<minm){
                    gb = b;
                    ga = a;
                    minm = pref[b] - pref[a-1];
                }
            }
            discomfort = ga - s;
            cur_price += minm * units;
            for(int j=1;j<=24;++j){
                if(j>=ga && j<=gb){
                    sol[i][j] = 1;
                }else{
                    sol[i][j] = 0;
                }
            }
        }
    }
    int isok = random_sol(n,this_day,discomfort);
    if(isok){
        cout << "Solution obtained by generating random matrix"<<endl;
    }
    for(int i=1;i<=n;++i){
        for(int j=1;j<=24;++j){
            cout << sol[i][j] << " ";
        }
        cout << endl;
    }
    if(isok==0){
        target -= cur_price;
        if(target<0){
            cout << endl << "Solution not possible" << endl;
            exit(1);
        }
    }
    }
    return 0;
}