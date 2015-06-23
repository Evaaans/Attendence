#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

struct excption_record
{
    string company;     // 1.[公司] 锐战
    string department;  // 2.[部门] 页游中心-PR部
    string id_number;   // 3.[工号] R2346
    string name;        // 4.[姓名] 柏丽萍
    string excp_date;   // 5.[异常日期] 2015-05-01 星期五
    string status;      // 6.[状态] 迟到、早退、缺考、旷工、加班
    string on_time;     // 7.[上班时间] 9:25
    string off_time;    // 8.[下班时间] 18:20
    string comment;     // 9.[备注] 默认为空
};

struct vacation_record
{
    string department;  // 1.[中心部门] 页游中心-美术部-原画组
    string name;        // 2.[姓名] 曾腾
    string apply_time;  // 3.[申请时间] 2015/5/7 9:59
    string period;      // 4.[请假时长] 8.0 小时
    string from;        // 5.[请假从] 2015/5/6 10:00
    string to;          // 6.[请假至] 2015/5/6 18:00
    string type;        // 7.[类别] 事假、年假、调休、病假
    string sick_leave;  // 8.[带薪病假] 1.5 小时
    string etc_leave;   // 9.[非带薪病假] 2.0 小时
    string approver;    // 10.[审批人] 叶志飞
};

ifstream fin;
ofstream fout;

vector<vacation_record> vacations;
vector<excption_record> excptions;

void splitBuf(string buf, vector<string> &part)
{
    unsigned long found = buf.find(',');

    while (found != string::npos)
    {
        part.push_back(buf.substr(0, found));
        buf = buf.substr(found + 1, buf.length());
        found = buf.find(',');
    }

    part.push_back(buf);
    
    for (int i = 0; i < part.size(); i++) cout << i << "\"" << part[i] << "\"" << endl;
}

void add_vacation(string buf)
{
    if (buf.length() < 1) return;
    
    vacation_record new_one;
    vector<string> part;
    part.push_back("---");
    splitBuf(buf, part);
    
    new_one.department = part[1];
    new_one.name = part[2];
    new_one.department = part[3];
    new_one.period = part[4];
    new_one.from = part[5];
    new_one.to = part[6];
    new_one.type = part[7];
    new_one.sick_leave = part[8];
    new_one.etc_leave = part[9];
    new_one.approver = part[10];
}

void add_excption(string buf)
{
    if (buf.length() < 1) return;
    
    excption_record new_one;
    vector<string> part;
    part.push_back("---");
    splitBuf(buf, part);
    
    new_one.company = part[1];
    new_one.department = part[2];
    new_one.id_number = part[3];
    new_one.name = part[4];
    new_one.excp_date = part[5];
    new_one.status = part[6];
    new_one.on_time = part[7];
    new_one.off_time = part[8];
    new_one.comment = part[9];
}

int main(int argc, char *argv[])
{
    fin.open("qingjia_utf8.csv");
    
    while (!fin.eof())
    {
        string vacation_buf;
        getline(fin, vacation_buf);
        add_vacation(vacation_buf);
    }
    
    fin.close();

    fin.open("yichang_utf8.csv");
    
    while (!fin.eof())
    {
        string vacation_buf;
        getline(fin, vacation_buf);
        add_excption(vacation_buf);
    }
    
    fin.close();


    fout.open("result.csv");
    fout.close();
    return 0;
}