/*main.cpp*/

//
// Lavithra Mysore
// COVID-19 Data Analysis
//

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <experimental/filesystem>
#include <locale>
#include <iomanip>
#include <vector>
#include <map>

using namespace std;
namespace fs = std::experimental::filesystem;


//
// getFilesWithinFolder
//
// Given the path to a folder, e.g. "./daily_reports/", returns 
// a vector containing the full pathnames of all regular files
// in this folder.  If the folder is empty, the vector is empty.
//


vector<string> getFilesWithinFolder(string folderPath)
{
  vector<string> files;

  for (const auto& entry : fs::directory_iterator(folderPath))
  {
    files.push_back(entry.path().string());
  }

  return files;
}

struct CountryData // this struct contains confirmed cov18 patients, deaths and recovered details.
{ 
     public:
         string confirmed;
         string deaths;
         string recovered;    
};

struct ContryFacts // it contains information of life expectancy and population and is stored in thier respective maps  
{ 
    public:
        int position;
        string value;
};

void readOneDailyReport(string filename,map<string,map<string, CountryData>>& data) // it opens the file in daily_report, loop and add all the data 
{ 
    ifstream inFS; // country wise to the map. If file has more refrence to one country then all its data is added and is presented as one
    inFS.open(filename);
    if (!inFS.is_open()) 
    {
        cout << "Could not open file " << filename << endl;
        exit(-1);
     }
    
    string line;
    getline(inFS, line);
    while(getline(inFS, line))
    {
        
        if (!inFS.fail())
        {
            if (line[0] == '"') // => province is "city, state"
	    {
                //
                // we want to delete the " on either end of the value, and
                // delete the ',' embedded within => will get city state:
                //
                line.erase(0, 1); // delete the leading "
                size_t pos = line.find(','); // find embedded ','
                line.erase(pos, 1);// delete ','
                pos = line.find('"');// find closing "
                line.erase(pos, 1);// delete closing "          
            }
            
            stringstream s(line);
            string province, country, last_update;
            string confirmed, deaths, recovered;
            
            getline(s, province, ',');
            getline(s, country, ',');
            getline(s, last_update, ',');
            getline(s, confirmed, ',');
            
            getline(s, deaths, ',');
            getline(s, recovered, ',');
            
            if (confirmed == "")
	    {
                confirmed = "0";
            }
            if (deaths == "")
	    {
                deaths = "0";
            }
            if (recovered == "")
   	    {
                recovered = "0";
            }
            if (country == "Mainland China") // map to name in the earlier reports:
	    {
                country = "China";
            }
            
            map<string, CountryData> value;
            
            if(data.count(country)==0)
	    {
                data.emplace(country,value); // add country name to map if it doesn't exist
            }
            if((data.at(country)).count(filename)==0) // add map value to map if it doesn't already exist
	    { 
                (data.at(country))[filename]={confirmed,deaths ,recovered};
            }
            else
	    {
                string s1,s2,s3;
                int i,j,k;
                s1=((data.at(country)).at(filename)).confirmed ;// add the respective data confirmed , deaths and recovered if 
                s2=((data.at(country)).at(filename)).deaths;// there are more than one refrence to a country
                s3=((data.at(country)).at(filename)).recovered; 
                i = stoi(s1)+stoi(confirmed);// stoi changes string to int
                ((data.at(country)).at(filename)).confirmed = to_string(i);
                j = stoi(s2)+stoi(deaths);
                ((data.at(country)).at(filename)).deaths = to_string(j);
                k = stoi(s3)+stoi(recovered);
                ((data.at(country)).at(filename)).recovered = to_string(k);

            }
        }

    }
    inFS.close();
    
}

void readWorldFacts(string filename, map<string,ContryFacts>& facts) // it takes in world facts and add data to coresponding maps
{
    ifstream inFS;
    inFS.open(filename);
    if (!inFS.is_open())
    {
        cout << "Could not open file wordFile.txt." << endl;
        exit(-1);
     }
    
    string line;
    getline(inFS, line);
    while(getline(inFS, line))
    {
        
        if (!inFS.fail())
	{
            if (line[0] == '"') // => province is "city, state"
	    {
                //
                // we want to delete the " on either end of the value, and
                // delete the ',' embedded within => will get city state:
                //
                line.erase(0, 1); // delete the leading "
                size_t pos = line.find(','); // find embedded ','
                line.erase(pos, 1);// delete ','
                pos = line.find('"');// find closing "
                line.erase(pos, 1);// delete closing "          
            }
            
            stringstream s(line);
            string position, country, value;
            
            getline(s, position, ',');
            getline(s, country, ',');
            getline(s, value, ',');
            
            if (value == ""){
                value = "0";
            }
            
            facts[country]={stoi(position),value};// country and coresponding value (population/life expectancy) is added
            
        }
    }
    inFS.close();
}
void help()
{ 
    cout<<"Available commands:\n";
    cout<<" <name>:enter a country name such as US or China\n";
    cout<<" countries:list all countries and most recent report\n";
    cout<<" top10:list of top 10 countries on most recent # of confirmed cases\n";
    cout<<" totals:world-wide totals of confirmed, deaths, recovered\n";
    cout<<" faq:Answers frequently asked questions related corona Virus\n";
}

void totals( map<string ,map<string, CountryData>> data) // it prints out the total deaths , confrimed and recovered
{
    
    map<string ,map<string, CountryData>>::iterator itr;
    map<string, CountryData>::reverse_iterator itr1;
    string date;
 
    itr = data.begin();
    itr1=(itr->second).rbegin();
    cout<<"As of "<<date.append(itr1->first,16,10)<<", the world-wide totals are:"<<endl;
    
    int Deaths=0;
    int Confirmed=0;
    int Recovered=0;
    for (itr = data.begin(); itr != data.end(); ++itr) // we loop around and add all countries latest deaths , confrimed and recovered
    {
        if((itr->second).count(itr1->first)==0) // and print it out
	{ 
          continue;
        }
        else
	{
          Deaths += stoi(((itr->second).at(itr1->first)).deaths);
          Confirmed += stoi(((itr->second).at(itr1->first)).confirmed);
          Recovered += stoi(((itr->second).at(itr1->first)).recovered);
        }
    }
    cout<<" confirmed: "<<Confirmed;
    cout<<endl<<" deaths: "<<Deaths<<" ("<<(float(Deaths)*100)/float(Confirmed)<<"%)";
    cout<<endl<<" recovered: "<<Recovered<<" ("<<(float(Recovered)*100)/float(Confirmed)<<"%)"<<endl;    
}

void top10(map<string ,map<string, CountryData>> data) // prints the top 10 data 
{
    map<string ,map<string, CountryData>>::iterator itr;
    map<string, CountryData>::reverse_iterator itr1;
    map<int, string> top_10;
    map<int, string>::reverse_iterator itr3;
    int count=0;
 
    itr = data.begin();
    itr1=(itr->second).rbegin(); // contain map to latest value 
    
    for (itr = data.begin(); itr != data.end(); ++itr) //loop around the map
    {
        if((itr->second).count(itr1->first)==0) // if the latest date data is not avaliable it skips the country
	{ 
          continue;
        }
        else
	{
            if(count<10) // takes in data for first 10 country , its value as key and name as value
	    { 
                top_10.emplace(stoi(((itr->second).at(itr1->first)).confirmed),itr->first);
            }
            else //after taking in first 10 country data, if we find a country data whose value is more than our's first member we delete
	    { 
                map<int, string>::iterator itr2; // our first and input the new country data
                itr2 =top_10.begin(); // as the loop goes on we have top 10 data in ascending order and map stores all data in ascending order
                if(itr2->first<stoi(((itr->second).at(itr1->first)).confirmed))
		{
                    top_10.erase(itr2->first);
                    top_10.emplace(stoi(((itr->second).at(itr1->first)).confirmed),itr->first);
                }

            }

            count++;
        }
    }
    count=1;
    for (itr3 = top_10.rbegin(); itr3 != top_10.rend(); ++itr3) // we print the data in reverse order from max to min value
    { 
        cout<<count<<". "<<itr3->second<<": "<<to_string(itr3->first)<<"\n";
        count++;
    }
    
    
    
}

void countries( map<string ,map<string, CountryData>> data) // loops and prints out latest data of all the country
{ 
      map<string ,map<string, CountryData>>::iterator itr;
      map<string, CountryData>::reverse_iterator itr1;
    
      itr = data.begin();
      itr1=(itr->second).rbegin();
    
      for (itr = data.begin(); itr != data.end(); ++itr) // if the country data is not avaliable for the latest date, 0,0,0 is printed
      {
          if((itr->second).count(itr1->first)==0)
	  {
              cout<< itr->first<< ": "<<0<< ", " <<0<< ", " <<0 << "\n";
              continue;
          }
          else
	  {
               cout<< itr->first 
               << ": "<< ((itr->second).at(itr1->first)).confirmed<< ", "
               << ((itr->second).at(itr1->first)).deaths<< ", " << ((itr->second).at(itr1->first)).recovered
               << "\n"; 
          }
      } 
    
}  

bool PrintCDR(int cdr,int &First7,int count1,bool &check,int Last7);
void TimeLine(string input,string command,map<string ,map<string, CountryData>> data); 

void countryName( map<string ,map<string, CountryData>> data, map<string,ContryFacts> lifeLimit,
                       map<string,ContryFacts> population,string command) // it prints out all the latest data, and facts about a country
{ 
    map<string, CountryData>::iterator itr; // like population,Life Expectancy ,Latest data(confirmed,recovered),First confirmed case
    map<string, CountryData>::reverse_iterator itr1; // and First recorded death
    
    string date;
    bool check=true;
    string input;
    
    itr1=(data.at(command)).rbegin();
    itr=(data.at(command)).begin();
    
    cout<<"Population: "<<(population.at(command)).value<<endl;
    cout<<"Life Expectancy: "<<(lifeLimit.at(command)).value<<" years\n";
    cout<<"Latest data:\n";
    cout<<" confirmed: "<<(itr1->second).confirmed<<endl;
    cout<<" deaths: "<<(itr1->second).deaths<<endl;
    cout<<" recovered: "<<(itr1->second).recovered<<endl;
    
    cout<<"First confirmed case: "<<date.append(itr->first,16,10)<<endl;//prints out first value avaliable on that country
    cout<<"First recorded death: ";

    for(itr=(data.at(command)).begin();itr !=(data.at(command)).end();++itr) // we loop and print out first data on death which is
    {
        if(stoi((itr->second).deaths)>0) // more than 0
 	{
           date="";
           cout<<date.append(itr->first,16,10)<<endl;
           check=false;
           break;
        }
    }
    if(check == true)
    {
        cout<<"none"<<endl;
    }
    
    cout<<"Do you want to see a timeline? Enter c/d/r/n> "; // it gives option for user to see time line
    getline(cin,input);
    TimeLine(input,command,data);
}

void TimeLine(string input,string command,map<string ,map<string, CountryData>> data)
{
    map<string,int> DateCount;
    map<string, CountryData>::iterator itr;
    int count=1;
    int count1=1;
    int count2=0;
    int count3=0;
    int count4=0;
    
    string date="";
    int i=(data.at(command)).size()-7;
    
    bool check1,check2,check3,check4;
    check1=false;
    check2=false;
    check3=false;
    check4=false;
    
    if(input=="c"){ cout<<"Confirmed:\n";}
    if(input=="d"){ cout<<"Deaths:\n";}
    if(input=="r"){ cout<<"Recovered:\n";}
    
    for(itr=(data.at(command)).begin();itr !=(data.at(command)).end();++itr) // we print out first 7 and last 7 data on their country
    {
        if((stoi((itr->second).confirmed)>0)||(check1==true)) // if the data is just 14 or less then we print all the data 
	{
           check1=true;
           DateCount[itr->first]=count;
            count++;
        }
        
        if((input=="c")&&(PrintCDR(stoi((itr->second).confirmed),count4,count1,check4,i)==true))
	{
            date="";
            cout<<date.append(itr->first,16,10)<<" (day "<<DateCount[itr->first]<<"): "<<(itr->second).confirmed<<endl;
        }

        if((input=="d")&&(PrintCDR(stoi((itr->second).deaths),count2,count1,check2,i)==true))
	{
            date="";
            cout<<date.append(itr->first,16,10)<<" (day "<<DateCount[itr->first]<<"): "<<(itr->second).deaths<<endl;
        }
            
        if((input=="r")&&(PrintCDR(stoi((itr->second).recovered),count3,count1,check3,i)==true))
	{
             date="";
             cout<<date.append(itr->first,16,10)<<" (day "<<DateCount[itr->first]<<"): "<<(itr->second).recovered<<endl;
        } 
         count1++;
    }
    
}  

bool PrintCDR(int cdr,int &First7,int count1,bool &check,int Last7) // it sees that we only print first 7 and last 7 
{
    if((cdr>0)||(check==true))
    {
        check=true;
        First7++;
        if((First7<=7)||(count1>Last7))
	{
            return true;
        }
        if(First7==15)
	{
            cout<<" . "<<endl<<" . "<<endl<<" ."<<endl;
            return false;
            
        }
        else
	{
            return false;
        }
    }
    return false;   
}

void faq() // answers frequently asked questions
{ 
    string options;
    
    cout<<"1. What is coronavirus?\n";
    cout<<"2. What are symptoms of coronavirus?\n";
    cout<<"3. How can i protect myself from infection?\n";
    cout<<"4. Does 2019-nCov affects older people or are younger people also susceptible?\n";
    cout<<"5. Can pets at home spread the 2019-nCoV?\n";
    cout<<"\n Which question's answer do you need (1,2 or 3 or if you want to exit 0)?";
    
    getline(cin, options);
    if(options=="0")
    {
        return;
    }
    if(options=="1")
    {
        cout<<"Coronaviruses are a large family that are known to cause illness\n";
        cout<<"from the common cold to most severe diseases such as MERS and SARS.\n";
    }
    if(options=="2")
    {
        cout<<"COVID-19 symptoms include:\n";
        cout<<"  -Cough\n";
        cout<<"  -Fever\n";
        cout<<"  -Shortness of breath\n";
        cout<<"In rare cases, COVID-19 can lead to severe respiratory problems, kidney failure or death.\n";
    }
    if(options=="3")
    {
        cout<<"Some measures you can take to avoid close contact with others include:\n";
        cout<<"  -Stay home as much as possible and reduce visitors.\n";
        cout<<"    -Practice social distancing:\n";
        cout<<"    -Stay at least six feet away from others in public places.\n";
        cout<<"  -Call friends and family or visit by video.\n";
        cout<<"  -Ask your employer if it’s possible to work from home.\n";
        cout<<"  -Avoid people who appear sick.\n";
        cout<<"  -Go grocery shopping and run errands during off-peak times.\n";
    } 
    if(options=="4")
    {
        cout<<"People of all ages can be infected by the virus.Older people and those with pre-existing\n";
        cout<<"medical conditions appear to be more vulnerable to becoming severely ill with the virus.\n";  
    } 
    if(options=="5")
    {
        cout<<"At present, there is no evidence that companion animals/pets such as dogs or cats can be infected with the virus.\n";
        cout<<"However, it is always a good idea to wash your hands with soap and water after contact with pets.\n";
 
    }
}

//
// main:
//
int main()
{
  map<string ,map<string, CountryData>> data;
  map<string,ContryFacts> lifeLimit;
  map<string,ContryFacts> population;
  map<string ,map<string, CountryData>>::iterator itr;
    
  int count=0;
  bool check=false;
    
  cout << "** COVID-19 Data Analysis **" << endl;
  cout << endl;
  cout << "Based on data made available by John Hopkins University" << endl;
  cout << "https://github.com/CSSEGISandData/COVID-19" << endl;
  cout << endl;
  
  //
  // setup cout to use thousands separator, and 2 decimal places:
  //
  cout.imbue(std::locale(""));
  cout << std::fixed;
  cout << std::setprecision(2);

  //
  // get a vector of the daily reports, one filename for each:
  //
  vector<string> files = getFilesWithinFolder("./daily_reports/");
    
   for (string filename: files)
   {
       readOneDailyReport(filename, data);// we loop and input all the data in map
       count++;
   }
   
  files.clear();
  files = getFilesWithinFolder("./worldfacts/");
  for (string filename: files) //we input all the world fact in lifeLimit and population map
  {
      if(filename=="./worldfacts/life_expectancies.csv")
      {
         readWorldFacts(filename, lifeLimit);  
      }
      else
      {
          readWorldFacts(filename, population); 
      }
      
  } 
  
  cout<<">> Processed "<<count<<" daily reports\n";
  cout<<">> Processed 2 files of world facts\n";
  cout<<">> Current data on "<<data.size()<<" countries\n";
    
    
  string command;
    
  cout<<"\nEnter command (help for list, # to quit)> ";
  getline(cin, command);
    
  while(command != "#") // loop and take in command until user inputs "#"
  { 
      if(command =="help")
      {
          help();
          check=true;
      }
      if(command=="totals")
      {
          totals(data);
          check=true;
      }
      if(command=="top10")
      {
          top10(data);
          check=true;
      }
      if(command=="countries")
      {
          countries(data); 
          check=true;
      }
      if(command=="faq")
      {
          faq(); 
          check=true;
      }
      if(data.count(command)==1)
      {
         countryName(data,lifeLimit,population,command);
         check=true;
      }
      if(check==false)
      {
          cout<<"country or command not found...\n";
      }

      check=false;
      cout<<"\nEnter command> ";
      getline(cin, command);
  }
    


  return 0;
}
