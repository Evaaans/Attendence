#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stdlib.h>

using namespace std;

struct date_time
{
    int yy;
    int mm;
    int dd;
    int hour;
    int min;
    int sec;
    string weeknum;
};

struct excption_record
{
    string company;         // 0.[公司] 锐战
    string department;      // 1.[部门] 页游中心-PR部
    string id_number;       // 2.[工号] R2346
    string name;            // 3.[姓名] 柏丽萍
    date_time excp_date;    // 4.[异常日期] 2015-05-01 星期五 (PATTERN_1)
    string status;          // 5.[状态] 迟到、早退、缺考、旷工、加班
    date_time on_time;      // 6.[上班时间] 9:25 (PATTERN_3)
    date_time off_time;     // 7.[下班时间] 18:20
    string comment;         // 8.[备注] 默认为空
};

struct vacation_record
{
    string department;      // 0.[中心部门] 页游中心-美术部-原画组
    string name;            // 1.[姓名] 曾腾
    date_time apply_time;   // 2.[申请时间] 2015/5/7 9:59 (PATTERN_2)
    double period;          // 3.[请假时长] 8.0 小时
    date_time from;         // 4.[请假从] 2015/5/6 10:00 (PATTERN_2)
    date_time to;           // 5.[请假至] 2015/5/6 18:00
    string type;            // 6.[类别] 事假、年假、调休、病假
    double sick_leave;      // 7.[带薪病假] 1.5 小时
    double etc_leave;       // 8.[非带薪病假] 2.0 小时
    string approver;        // 9.[审批人] 叶志飞
};

const string PATTERN_1 = "yyyy-mm-dd weeknum";
const string PATTERN_2 = "yyyy/mm/dd hour:min";
const string PATTERN_3 = "hour:min";

ifstream fin;
ofstream fout;

vector<vacation_record> vacations;
vector<excption_record> excptions;

// 按分隔符拆分到字符串数组
void splitBuf(string buf, vector<string> &part, char delim)
{
    if (buf.length() < 1) return;

    unsigned long found = buf.find(delim);

    part.clear();

    while (found != string::npos)
    {
        part.push_back(buf.substr(0, found));
        buf = buf.substr(found + 1, buf.length());
        found = buf.find(delim);
    }

    part.push_back(buf);
    
    for (int i = 0; i < part.size(); i++) cout << i << "\"" << part[i] << "\"" << endl;
}

// 拆分小时数
double splitPeriod(string buf)
{
    if (buf.length() < 1) return 0;

    unsigned long found = buf.find(' ');

    //cout << "@@@@@@@@@@@@@" << atof(buf.substr(0, found).c_str()) << endl;

    return atof(buf.substr(0, found).c_str());
}

// 拆分日期或时间
date_time splitDateTime(string buf, string pattern)
{
    date_time rtn;
    
    if (buf.length() < 1) return rtn;
    
    vector<string> part;

    if (pattern == PATTERN_1) // 2015-05-01 星期五
    {
        splitBuf(buf, part, ' ');
        rtn.weeknum = part[1];

        splitBuf(part[0], part, '-');
        rtn.yy = atoi(part[0].c_str());
        rtn.mm = atoi(part[1].c_str());
        rtn.dd = atoi(part[2].c_str());
    }
    else if (pattern == PATTERN_2) // 2015/5/7 9:59
    {
        splitBuf(buf, part, ' ');
        string date_buf = part[0];
        string time_buf = part[1];

        splitBuf(date_buf, part, '/');
        rtn.yy = atoi(part[0].c_str());
        rtn.mm = atoi(part[1].c_str());
        rtn.dd = atoi(part[2].c_str());

        splitBuf(time_buf, part, ':');
        rtn.hour = atoi(part[0].c_str());
        rtn.min = atoi(part[1].c_str());
    }
    else if (pattern == PATTERN_3) // 9:25
    {
        splitBuf(buf, part, ':');
        rtn.hour = atoi(part[0].c_str());
        rtn.min = atoi(part[1].c_str());
    }
    else
    {
        cout << "Unknown date time pattern: " << pattern << endl;
        cout << "\"" << buf << "\"" << endl;
    }

    return rtn;
}

void read_vacation(string buf)
{
    if (buf.length() < 1) return;
    if (buf.find("中心部门") == 0) return;
    
    vacation_record new_one;
    vector<string> part;
    splitBuf(buf, part, ',');
    
    new_one.department = part[0];
    new_one.name = part[1];
    new_one.apply_time = splitDateTime(part[2], PATTERN_2);
    new_one.period = splitPeriod(part[3]);
    new_one.from = splitDateTime(part[4], PATTERN_2);
    new_one.to = splitDateTime(part[5], PATTERN_2);
    new_one.type = part[6];
    new_one.sick_leave = splitPeriod(part[7]);
    new_one.etc_leave = splitPeriod(part[8]);
    new_one.approver = part[9];
}

void read_excption(string buf)
{
    if (buf.length() < 1) return;
    if (buf.find("公司") == 0) return;
    
    excption_record new_one;
    vector<string> part;
    splitBuf(buf, part, ',');
    
    new_one.company = part[0];
    new_one.department = part[1];
    new_one.id_number = part[2];
    new_one.name = part[3];
    new_one.excp_date = splitDateTime(part[4], PATTERN_1);
    new_one.status = part[5];
    new_one.on_time = splitDateTime(part[6], PATTERN_3);    // 可能为空
    new_one.off_time = splitDateTime(part[7], PATTERN_3);   // 可能为空
    new_one.comment = part[8];
}

int main(int argc, char *argv[])
{
    // 读请假表
    fin.open("qingjia_utf8.csv");
    while (!fin.eof())
    {
        string buf;
        getline(fin, buf);
        read_vacation(buf);
    }
    fin.close();

    // 读异常表
    fin.open("yichang_utf8.csv");
    while (!fin.eof())
    {
        string buf;
        getline(fin, buf);
        read_excption(buf);
    }
    fin.close();

    // 生成结果
    fout.open("result.csv");
    fout.close();
    return 0;
}