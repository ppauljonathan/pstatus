#include <iostream>
#include <unistd.h>
#include <fstream>
#include <string>
#include <vector>
#include <ctime>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/types.h> 
#include <fcntl.h>
#include <errno.h>
#include <linux/wireless.h>
#include <alsa/asoundlib.h>

using namespace std;

string version="v2.0.1",authour="P Paul Jonathan";

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
    struct iwreq wreq;
    string ssid_data;
    int sockfd;
    char * id;

    memset(&wreq, 0, sizeof(struct iwreq));
    sprintf(wreq.ifr_name, "wlo1");

    if((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
        cout<<" INTERNET ERROR";
    }
    char buffer[32];
    memset(buffer, 0, 32);
    wreq.u.essid.pointer = buffer;
    wreq.u.essid.length = 32;
    if (ioctl(sockfd,SIOCGIWESSID, &wreq) == -1) {
        cout<<" INTERNET ERROR";
    }
    wreq.u.essid.length;
    if(wreq.u.essid.length == 0){cout<<" NO INTERNET";}
    else{printf(" NET %s",wreq.u.essid.pointer);}
    cout<<" |";
}

void help(){
    cout<<"pstatus "<<version<<" "<<authour<<"\n";
    cout<<"Usage: pstatus [options]\n";
    cout<<"Options:\n";
    cout<<"\t-h\tDisplay this information and exit.\n";
    cout<<"\t-v\tDisplay verion and exit.\n";
    cout<<"\t-o\tDisplay system status once and exit.\n";
    cout<<"\t-brt\tDisplay monitor brightness percentage and exit.\n";
    cout<<"\t-bat\tDisplay remaining battery percentage and exit.\n";
    cout<<"\t-vol\tDisplay volume percentageand exit.\n";
    cout<<"\t-net\tDisplay ssid of current network connection and exit.\n";
    cout<<"\t-dat\tDisplay date and time and exit.\n";
    cout<<"\tnone\tDisplay system status (updates every 2 seconds),\n\t \tcan be used as status bar.";
}

int main(int argc,char **argv){
	string option=argv[1]?argv[1]:"";
	if(option=="-h"){
        help();
	}else if(option=="-v"){
        cout<<version<<"\n";
    }else if(option=="-brt"){
        brightness();
    }else if(option=="-bat"){
        cout<<"|";battery();
    }else if(option=="-vol"){
        cout<<"|";volume();
    }else if(option=="-net"){
        cout<<"|";ssid();
    }else if(option=="-dat"){
        cout<<"|";dt();
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
