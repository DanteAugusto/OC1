#include <string>
#include <vector>
#include <iostream>

std::vector<std::string> tokenize(std::string s){
    std::vector<std::string> tokens;
    int n = s.length();
    int count = 0;
    int start = 0;
    //add $3 $2 $1
    for(int i = 0; i < n; i++){
        if(s[i] == ' '){
            std::string t = s.substr(start,count);
            tokens.push_back(t);
            count = 0;
            start = i+1;
        }else{
            count++;
        }
    }
    std::string t = s.substr(start, n - start);
    tokens.push_back(t);
    return tokens;
}