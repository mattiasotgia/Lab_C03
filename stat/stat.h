#ifndef _STAT_H
#define _STAT_H

#include <vector>

class stat{
public:
    stat();

    virtual ~stat();

    void read_values(std::vector<double> &v)
    {
        clear();
        _v = v;
    }

    double avg();
    double std_err();
    void clear();

private:
    std::vector<double> _v;
    double sqr_avg();
    double variance();
    double emp_var();
    double std();
    double std_adp();
};

#endif
