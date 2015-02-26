#ifndef CMD_H
#define CMD_H

#include <string>
#include <QStringList>

class Cmd
{
public:
  void end();
  bool delimLF(string);
  string getid();
  string getline();
  string getparms(bool s=false);
  void init(char *,int);
  bool more();

  string remws(string s);
  void markpos();
  void rewindpos();

  QStringList bsplits();
  QStringList qsplits();
  vector<string> ssplits();

private:
  void skippast(char c);
  void skips(string s);
  void skiptows();

  size_t bgn;
  size_t len;
  size_t pos;
  size_t pos0;
  string str;
};

QStringList bsplit(string s);
QStringList qsplit(string s,bool p=false);
vector<string> ssplit(string s);

#endif
