#include<iostream>
#include<string>
#include<algorithm>
#include<cmath>
#include<vector>
#include <sstream>
#include <fstream>  
#include <iomanip>   

using namespace std;

//functions for base conversions
void convert_to_base_10(string &str1, string &str2, int base1){
    if(base1==10){
        str2=str1;
        return;
    }
    string digits="0123456789ABCDEF";
    if(base1<2||base1>17){
        str2="ERROR!!: program only supports bases from 2 to 16";
        return;
    }
    
    for (int i = 0; i < str1.length(); i++) {
    if ((digits.substr(0,base1)).find(str1[i]) == string::npos && str1[i] != '.') {
        str2 = "ERROR!!: an entry in your input is invalid for the given base";
       return;
    }
    }
     double result=0;
    //finding the integer part
   size_t point=str1.find("."); //decimal point
   if (point == string::npos) point = str1.length();
   int int_part = static_cast<int>(point) - 1;
   str2="";
   
   double i=1;
   while(int_part>=0){
    if(str1[int_part]>='A') result+=((str1[int_part])-'A'+10)*i;
    else result+=((str1[int_part])-'0')*i;
    i=base1*i;
    int_part--;
   }
   if (point == string::npos) return;

   //finding the fractional part
   size_t fract_part=point+1;
   i=1.0/base1;
   while(fract_part<str1.length()){
    if(str1[fract_part]>='A') result+=((str1[fract_part])-'A'+10)*i;
    else result+=((str1[fract_part])-'0')*i;
    fract_part++;
    i=(i)/(base1);
   } 
//string output
  ostringstream out;
    out.precision(5);
    out << fixed << result;
    str2 = out.str();
}

void convert_from_base_10(string &str1, string &str2, int base2){
    if(base2==10){
        str2=str1;
        return;
    }
    string digits="0123456789ABCDEF";
    if(base2<2||base2>17){
        str2="ERROR!!: program only supports bases from 2 to 16";
        return;
    }

   
    size_t point=str1.find(".");
    string int_part=str1.substr(0,point);
    int myint=stoi(int_part);
    str2="";
    //converting the integer part of string to int
    if(myint==0) {
        str2="0";
    }
    while(myint!=0){
        str2=digits[myint%base2]+str2;
        myint=myint/base2;
    }

    //converting the fraction part of string 
    int precision=5; //setting the fractional precision to 5 digits
    double myfract;
    if(point == string::npos)  return;
    string fract_part = str1.substr(point + 1);
    myfract = stod("0." + fract_part);  
    if(myfract!=0) str2+='.';
    else{
        str2+='.';
        str2+='0';
        str2+='0';
    }
    while(precision--&&myfract>0){
        myfract=myfract*base2;
        int digit=(int)myfract;
        str2+=digits[digit];
        myfract=myfract-digit;
    }
    }

void convert_base(string &str1, string &str2,int base1, int base2){
    //handling invalid digits and unsupported bases
    string str_decimal;
    convert_to_base_10(str1, str_decimal,base1);
    convert_from_base_10(str_decimal,str2,base2);
    }

//functions for arithmetic operations
void unsigned_addition(string &str1,string &str2,string &result,int bits){
    result="";
    int carry=0;
    for(int i=bits-1;i>=0;i--){
       int sum=str1[i]-'0' +str2[i]-'0'+carry;
       result+=char((sum)%2+'0');
       carry=(sum/2);
    }
    reverse(result.begin(),result.end());
    if(carry==1) result="ERROR:overflow";
}
void unsigned_subtraction(string &str1,string &str2,string &result,int bits){
    result="";
    int borrow=0;
    for(int i=bits-1;i>=0;i--){
        int sub=(str1[i]-'0')-(str2[i]-'0')-borrow;
        if(sub<0){
            sub=sub+2;
            borrow=1;
        }
        else borrow=0;
        result+=(sub+'0');
    }
    if(borrow==1) {
    result="ERROR:underflow";
    return;}
    else reverse(result.begin(),result.end());
}

void unsigned_multiplication(string &str1,string &str2,string &result,int bits){
    int n=bits;
    result=string(2*n,'0');

    for (int i=n-1;i>=0;i--) {
        if (str2[i]=='1') {

            string temp = str1 + string(n - 1 - i, '0');
            if ((int)temp.length() < 2 * n)
                temp=string(2*n-temp.length(),'0')+temp;
            int carry=0;
            for (int j=2*n-1;j>=0;j--) {
                int sum=(result[j]-'0')+(temp[j]-'0')+carry;
                result[j]=(sum%2)+'0';
                carry=sum/2;
            }
        }
    }
}

void unsigned_division(string &str1,string &str2,string &result,int bits){
    if(str2==string(bits,'0')){
        result="ERROR!!can't divide by 0";
        return;
    }

    string remainder=string(bits,'0');
    result=string(bits,'0');
    for(int i=0;i<bits;i++){
        remainder=remainder.substr(1)+str1[i];
        if(remainder>=str2){
            string copy=remainder;
            unsigned_subtraction(copy,str2,remainder,bits);
            result[i]='1';
        }
    }
}

void to_1s_complement(string &str) {
    // if negative
    int n=str.length();
    for (int i=0;i<n;i++) { 
            if(str[i]=='1') str[i]='0';
            else str[i]='1';
        }
}

void to_2s_complement(string &str){
    to_1s_complement(str);
    string copy=str;
    int bit = str.length();
    string one(bit, '0');
    one[bit - 1] = '1';
    unsigned_addition(copy,one,str,bit);
}

void arithmetic_operation(string &str1,string &str2, string &result, int bit_sys,int rep_type,int operation){
    string digits="01";
    switch(rep_type){
        case 1:{
        //unsigned
            switch(operation){
                case 1:{
                    //addition
                    unsigned_addition(str1,str2,result,bit_sys);  
                    break;          
                }
                case 2:{
                    //subtarction
                    unsigned_subtraction(str1,str2,result,bit_sys);
                    break;
                }
                case 3:{
                    //multiplication
                    unsigned_multiplication(str1,str2,result,bit_sys);
                    break;
                }
                case 4:{
                    //division
                    unsigned_division(str1,str2,result,bit_sys);
                    break;
                }
            }
        }
        break;
        case 2:{
        //signed-magnitude
            switch(operation){
                case 1:{
                    //addition
                    //same sign
                    if(str1[0]==str2[0]){
                        string s1=str1.substr(1);
                        string s2=str2.substr(1);
                        unsigned_addition(s1,s2,result,bit_sys-1);
                        if(result!="overflow") result.insert(result.begin(),str1[0]);
                        else result="overflow";
                    }

                    //diff sign
                    else{
                        string s1=str1.substr(1);
                        string s2=str2.substr(1);
                        if(s1>s2) {
                            unsigned_subtraction(s1,s2,result,bit_sys-1);
                            result.insert(result.begin(),str1[0]);
                        }
                        else if(s1<s2){
                            unsigned_subtraction(s2,s1,result,bit_sys-1);
                            result.insert(result.begin(),str2[0]);
                        }
                        else result = string(bit_sys,'0');
                    }
                    break;
                }
                case 2:{
                    //subtraction
                    if(str1[0]!=str2[0]){
                        string s1=str1.substr(1);
                        string s2=str2.substr(1);
                        unsigned_addition(s1,s2,result,bit_sys-1);
                        if(result!="overflow") result.insert(result.begin(),str1[0]);
                    }

                    //same sign
                    else{
                        string s1=str1.substr(1);
                        string s2=str2.substr(1);
                        if(s1>s2) {
                            unsigned_subtraction(s1,s2,result,bit_sys-1);
                            result.insert(result.begin(),str1[0]);
                        }
                        else if(s1<s2){
                            unsigned_subtraction(s2,s1,result,bit_sys-1);
                            char flipped=(str1[0]=='0')?'1':'0';
                            result.insert(result.begin(),flipped);
                        }
                        else result = string(bit_sys,'0');
                    }
                    break;
                }
                case 3:{
                    //multiplication
                    if(str1==string(bit_sys,'0')||str2==string(bit_sys,'0')){
                        result=string(bit_sys,'0');
                         return;
                    };
                     if(str1[0]!=str2[0]){
                        string s1=str1.substr(1);
                        string s2=str2.substr(1);
                        unsigned_multiplication(s1,s2,result,bit_sys-1);
                        result.insert(result.begin(),'1');
                    }

                    //same sign
                    else{
                        string s1=str1.substr(1);
                        string s2=str2.substr(1);
                        unsigned_multiplication(s1,s2,result,bit_sys-1);
                        result.insert(result.begin(),'0');
                    }
                    break;
                }
                case 4:{
                    //divison
                     if(str2==string(bit_sys,'0')) {
                        result="ERROR!!can't divide by 0";
                        return;
                    };
                    if(str1[0]!=str2[0]){
                        string s1=str1.substr(1);
                        string s2=str2.substr(1);
                        unsigned_division(s1,s2,result,bit_sys-1);
                        if(result!=string(bit_sys-1,'0')) result.insert(result.begin(),'1');
                    }

                    //same sign
                    else{
                        string s1=str1.substr(1);
                        string s2=str2.substr(1);
                        unsigned_division(s1,s2,result,bit_sys-1);
                        if(result!=string(bit_sys-1,'0')) result.insert(result.begin(),'0');
                    }
                    break;
                }
                break;
        }
        }
        break;
        case 3:{
        //1's compliment 
             switch(operation){
                case 1:{
                 //addition
                    int n=bit_sys;
                    string sum(n,'0');
                    int carry=0;

                    for (int i=n-1;i>=0;i--) {
                    int s=(str1[i]-'0')+(str2[i]-'0')+carry;
                    sum[i]= (s%2)+'0';
                    carry=s/2;
                    }

                    if (carry == 1) {
                    for (int i=n-1;i>=0&&carry;i--) {
                        int s=(sum[i]-'0')+carry;
                        sum[i]=(s%2)+'0';
                        carry = s/2;
                    }
                }
                if(str1[0]==str2[0]&&sum[0]!=str1[0]) result="ERROR:overflow";
                else result = sum;
                break;
                }
                case 2:{
                    //subtarction
                    string copy_str2=str2;
                    to_1s_complement(copy_str2);
                    int n=bit_sys;
                    string sum(n,'0');
                    int carry=0;

                    for (int i=n-1;i>=0;i--) {
                    int s=(str1[i]-'0')+(str2[i]-'0')+carry;
                    sum[i]= (s%2)+'0';
                    carry=s/2;
                    }

                    if (carry == 1) {
                    for (int i=n-1;i>=0&&carry;i--) {
                        int s=(sum[i]-'0')+carry;
                        sum[i]=(s%2)+'0';
                        carry = s/2;
                    }
                    }
                    if(str1[0]==str2[0]&&sum[0]!=str1[0]) result="ERROR:overflow";
                    else result = sum;
                    break;
                }
                case 3:{
                    //multiplication
                    if(str1==string(bit_sys,'0')||str2==string(bit_sys,'0')) {
                        result=string(bit_sys,'0');
                        return;
                    };
                    bool oppsign=false;
                    if(str1[0]!=str2[0]) oppsign=true;
                    if(str1[0]=='1') to_1s_complement(str1);
                    if(str2[0]=='1') to_1s_complement(str2);

                    unsigned_multiplication(str1,str2,result,bit_sys);
                    if(oppsign==true) to_1s_complement(result);
                    break;
                }
                case 4:{
                    //divison
                    if(str2==string(bit_sys,'0')) {
                        result="ERROR!!can't divide by 0";
                        return;
                    };
                    bool oppsign=false;
                    if(str1[0]!=str2[0]) oppsign=true;
                    if(str1[0]=='1') to_1s_complement(str1);
                    if(str2[0]=='1') to_1s_complement(str2);

                    unsigned_division(str1,str2,result,bit_sys);
                    if(oppsign) to_1s_complement(result);
                    break;
                }
                break;
            }
        }
        break;
        case 4:{
        //2s compliment
            switch(operation){
                case 1:{
                    //addition
                    int n=bit_sys;
                    string sum(n,'0');
                    int carry=0;
                    int carry_into_msb=0;

                    for (int i=n-1;i>=0;i--) {
                    int s=(str1[i]-'0')+(str2[i]-'0')+carry;
                    sum[i]=char((s%2)+'0');
                    carry=s/2;

                    if(i==0) carry_into_msb=carry;
                    }

                    int carryout=carry;
                    if(carryout!=carry_into_msb)  result="ERROR:overflow";
                    else result=sum;
                    break;                    
                }

                case 2:{
                    //subtarction
                    string copy_str2=str2;
                    to_2s_complement(copy_str2);
                    int n=bit_sys;
                    string sum(n,'0');
                    int carry=0;
                    int carry_into_msb=0;

                    for (int i=n-1;i>=0;i--) {
                    if(i==0) carry_into_msb=carry;
                    int s=(str1[i]-'0')+(copy_str2[i]-'0')+carry;
                    sum[i]= (s%2)+'0';
                    carry=s/2;
                    }
                    int carryout=carry;
                    if(carryout!=carry_into_msb) result="ERROR:overflow";
                    else result=sum;
                    break;
                }
                case 3:{
                    //multiplication
                    bool oppsign=false;
                    string s1=str1;
                    string s2=str2;
                    if(str1[0]!=str2[0]) oppsign=true;
                    if(str1[0]=='1') {
                        string copy=s1;
                        string one=string(bit_sys-1,'0');
                        one.push_back('1');
                        unsigned_subtraction(copy,one,s1,bit_sys);
                        to_1s_complement(s1);
                        }
                    if(str2[0]=='1') {
                        string copy=s2;
                        string one=string(bit_sys-1,'0');
                        one.push_back('1');
                        unsigned_subtraction(copy,one,s2,bit_sys);
                         to_1s_complement(s2);
                        }

                    unsigned_multiplication(s1,s2,result,bit_sys);
                    if(oppsign) to_2s_complement(result);
                    break;
                }
                case 4:{
                    //divison
                    string s1=str1;
                    string s2=str2;
                    bool oppsign=false;
                    if(str1[0]!=str2[0]) oppsign=true;
                    if(str1[0]=='1') 
                    {
                     string copy=s1;
                        string one=string(bit_sys-1,'0');
                        one.push_back('1');
                        unsigned_subtraction(copy,one,s1,bit_sys);
                        to_1s_complement(s1);
                    }
                    if(str2[0]=='1'){
                         string copy=s2;
                        string one=string(bit_sys-1,'0');
                        one.push_back('1');
                        unsigned_subtraction(copy,one,s2,bit_sys);
                         to_1s_complement(s2);
                    }
                    unsigned_division(s1,s2,result,bit_sys);
                    if(oppsign) to_2s_complement(result);
                    break;
                }

                break;
            }
        }
            break;
    }


}

 bool compare_numbers(const string &original,const string &roundtrip) {
    
   for(int i=0;;i++) {
    if(original[i]!=roundtrip[i]) return false;
    if(original[i]=='.'){
        if(original[i+1]==roundtrip[i+1])
        return true;
    }
 }
}


int main(){
    bool go_on=true;
    while(go_on){
        cout<<"Enter a number"<<endl;
        cout<<"1 for Base conversion\n";
        cout<<"2 for Binary arithmetic"<<endl;
        cout<<"3 to Exit"<<endl;

        int choice;
        cin>>choice;
        switch(choice){
            case 1: {
                string input;
                string output;
                int base_1;
                int base_2;
                cout<<"ENTER YOUR NUMBER TO CONVERT : "<<endl;
                cin>>input;
                cout<<"ENTER THE BASE OF THE INPUT NUMBER : "<<endl;
                cin>>base_1;
                cout<<"ENTER THE CONVERSION BASE : "<<endl;
                cin>>base_2;
                cout<<endl;

                convert_base(input, output, base_1,base_2);
                cout<<"Your converted number is :"<<output<<endl;
            }
            break;
            case 2: {
                string result;
                string str1,str2;
                int num_bits, rep_type, operation;
                
                cout<<"ENTER 1 FOR UNSIGNED REPRESENTATION, 2 FOR SIGNED REPRESENTATION : "<<endl;
                cin>>rep_type;
                cout<<"ENTER THE NUMBER OF BITS IN EACH OF YOUR NUMBER : "<<endl;
                cin>>num_bits;
                cout<<"ENTER YOUR FIRST BINARY NUMBER : "<<endl;
                cin>>str1;
                cout<<"ENTER YOUR SECOND BINARY NUMBER : "<<endl;
                cin>>str2;      
                cout<<"ENTER 1 FOR ADDITION"<<endl;
                cout<<"ENTER 2 FOR SUBTRACTION"<<endl;
                cout<<"ENTER 3 FOR MULTIPLICATION"<<endl;
                cout<<"ENTER 4 FOR DIVISION : "<<endl;
                cin>>operation;

                arithmetic_operation(str1,str2, result, num_bits, rep_type, operation);
                cout<<"Your result is : "<<result<<endl;
                cout<<endl;

            }
            break;
            case 3: go_on=false;
        }
        
    }
    return 0;
}


   