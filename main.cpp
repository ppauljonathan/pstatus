#include<iostream>
#include<unistd.h>
#include<fstream>
#include<string>
#include<vector>
#include<ctime>
using namespace std;

string version="v1.0.1",authour="P Paul Jonathan";

vector <string> split_string(string s,char delim){
    auto j=s.cbegin();
    vector<string> split;
    string k;
    for(auto i=s.cbegin();i!=s.cend()+1;i++){
        if(*i==delim||i==s.end()){
            k.erase();
            while(j!=i){k.push_back(*j);j++;}
            j=i+1;
            split.push_back(k);
        }
    }
    return split;
}

void brightness(){
    ifstream filw;
    string brec;
    int max_brightness,cur_brightness;
    filw.open("/sys/class/backlight/intel_backlight/max_brightness");
    if(filw.is_open()){filw>>brec;}
    filw.close();
    max_brightness=stoi(brec);
    brec.erase();
    filw.open("/sys/class/backlight/intel_backlight/brightness");
    if(filw.is_open()){filw>>brec;}
    filw.close();
    cur_brightness=stoi(brec);
    cout<<"| BRT "<<(cur_brightness*100)/max_brightness<<"% |";
}

void battery(){
    ifstream bat_file;
    bat_file.open("/sys/class/power_supply/BAT0/uevent");
    string s;
    char p[64];
    vector<string> bat_data;
    int batt_chg_perc;
    while(!bat_file.eof()){
        bat_file>>s;
        bat_data=split_string(s,'=');
        if(*bat_data.begin()=="POWER_SUPPLY_STATUS"){
            if(*bat_data.rbegin()=="Charging"){cout<<" CHR ";}else{cout<<" BAT ";}
        }
        if(*bat_data.begin()=="POWER_SUPPLY_CAPACITY"){
            cout<<*bat_data.rbegin()<<"% |";
        }
        
    }
    bat_file.close();
}

void volume(){
    FILE *vol=popen("pamixer --get-volume-human","r");
    cout<<" VOL ";
    char c;
    while((c=fgetc(vol))!='\n'){cout<<c;}
    cout<<" |";
    fclose(vol);
}

void dt(){
    string day_name[]{"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
    time_t now=time(0);
    tm *lt=localtime(&now);
    string mon,min;
    if(lt->tm_mon<10){
    }
    cout<<" "
        <<day_name[lt->tm_wday]
        <<" "
        <<lt->tm_mday
        <<"/"
        <<lt->tm_mon+1
        <<"/"
        <<lt->tm_year+1900
        <<" "
        <<lt->tm_hour
        <<":"
        <<lt->tm_min
        <<" |";
}

void ssid(){
    FILE *net=popen("iw wlo1 info | cut -d '\n' -f 5","r");
    char c;
    string ssid_data;
    while((c=fgetc(net))!='\n'){
        if(c!='\t')ssid_data.push_back(c);
    }

    do{
        ssid_data.erase(ssid_data.begin());
    }while(ssid_data[0]!=' ');
    if(ssid_data==" managed"){cout<<" NO INTERNET";}
    else{cout<<" NET"<<ssid_data;}
    cout<<" |";
    fclose(net);
}

void help(){
    cout<<"pstatus "<<version<<" "<<authour<<"\n";
    cout<<"Usage: pstatus [options]\n";
    cout<<"Options:\n";
    cout<<"\t-h\tDisplay this information and exit.\n";
    cout<<"\t-v\tDisplay verion and exit.\n";
    cout<<"\t-o\tDisplay system status once and exit.\n";
    cout<<"\tnone\tDisplay system status (updates every 2 seconds),\n\t \tcan be used as status bar.";
}

int main(int argc,char **argv){
	string option=argv[1]?argv[1]:"";
	if(option=="-h"){
        help();
	}else if(option=="-v"){
        cout<<version<<"\n";
    }else if(option=="-o"){
        cout<<"\r";
        brightness();
        battery();
        volume();
        ssid();
        dt();
	}else{
	    while(1){
    	    brightness();
        	battery();
	        volume();
	        ssid();
	        dt();
	        cout<<"\r";
	        fflush(stdout);
	        sleep(2);
	    }
	}
	return 0;
}
