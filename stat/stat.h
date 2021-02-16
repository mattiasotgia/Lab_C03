#ifndef _STAT_H
#define _STAT_H

// This code is completely open-source, 
// any feature addition is welcome!

// stat library, v1.0 as of 2021-02-16

#include <vector>

class stat{
public:
    stat();

    virtual ~stat();

    void read_values(std::vector<double> &v)
    {
        clear(); // Zeroing vector before new assignment
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
