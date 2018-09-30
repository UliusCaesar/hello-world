#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>

typedef struct person
{
    std::string name;
    int id;
    int sex;
} Person;

int getId(const std::string &str, int pos)
{
    char ch[50];
    int i;
    for(i = pos; str[i] != '@'; i++)
        ch[i - pos] = str[i];
    ch[i] = '\0';
    return atoi(ch);
}

int getSex(const std::string &str) {
    if (str[6] == 'M')
        return 1;
    else {
        if (str[6] == 'F')
            return 2;
        else return 0;
    }
}

void getName(const std::string &str, std::string &name) {
    int i;
    for(i = 7; str[i] != '/'; i++) // c 8 символа ( от 0 до 7) читаем Имя пользователя
    {
        if (str[i] != '"') {
            name.push_back(str[i]);
        }
    }
    
    for(i++ ; str[i] != '/'; i++) {
        if (str[i] != '"')
            name.push_back(str[i]);
    }
}

int main()
{
    std::ifstream fin("/Users/caesar_julia/Desktop/LP/MARINE.ged"); // потоки файлов , ввод вывод
    std::ofstream fout("/Users/caesar_julia/Desktop/LP/generated.pl");
    std::vector<Person> persons;
    std::string str;
    Person temp;
    
    if (fin.is_open())
    {
        while (std::getline(fin, str)) //ищем id
        {
            if (str[0] == '0' && str[3] == 'I')  // до 27 пропуск
                break;
        }
        
        do
        {
            if (str[0] == '0') {
                if (str[3] == 'I') {
                    temp.id = getId(str, 4);
                } else {
                    break;
                }
            }
            else if (str[0] == '1')
            {
                if (str.substr(2, 4) == "NAME") // С второго символа сравнивает строчку размером 4 , ищет NAME
                {
                    getName(str, temp.name);
                    
                } // getsex написать + инфа о детях
            }
            
            if (str.substr(2, 3) == "SEX")
            {
                temp.sex = getSex(str);
                
                
                persons.push_back(temp); //записываем в массив persons маленькие контейнеры temp
                temp.name.clear(); //очищаем маленький контейнер , внутри которого контейнер с Именем
            }
        }
        
        //выхожу из цикла do , тк не нашла 3 символа 'I', перехожу на строку с str[0] = 0 && str[3] = F
        while (std::getline(fin, str));
        
        int husb_id = -1; //отец
        int wife_id = -1; //мать
        std::vector<int> childs;
        while(std::getline(fin, str)) {
            if (str[0] == '1') {
                std::string substr = str.substr(2, 4);
                if (substr == "HUSB") {
                    husb_id = getId(str, 9);
                } else if (substr == "WIFE") {
                    wife_id = getId(str, 9);
                } else if (substr == "CHIL") { // считываем с файла , не надо инициализировать
                    childs.push_back(getId(str, 9));
                }
                
            }
            else if (str[0] == '0') {
                for (int i = 0; i < childs.size(); i++) {
                    if ((husb_id != -1)&&(wife_id != -1)) {
                        int k = 0;
                        int f = 0;
                        int m = 0;
                        for (int j = 0; j < persons.size(); j++){
                            if ( persons[j].id == childs[i])
                                k = j;
                            
                        }
                        for (int F = 0; F < persons.size(); F++)
                        if ( persons[F].id == husb_id){
                            f = F;
                            
                        }
                        for (int M = 0; M < persons.size(); M++)
                        if ( persons[M].id == wife_id){
                            m = M;
                            
                        }
                        
                        
                        fout << "father(\"";
                        fout << persons[f].name << "\", \"";
                        fout << persons[k].name << "\").\n";
                        
                        fout << "mother(\"";
                        fout << persons[m].name << "\", \"";
                        fout << persons[k].name << "\").\n";
                    }
                }
                husb_id = -1;
                wife_id = -1;
                childs.clear();
            }
        }
        fin.close();
        fout.close();
        return 0;
    }
    else {
        std::cout << "Unable to open file";
        return 0;
    }
}
