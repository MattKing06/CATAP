#ifndef _RUNNING_STATS_H_
#define _RUNNING_STATS_H_

#include <iostream>

class RunningStats
{
    // thanks to https://www.johndcook.com/blog/standard_deviation/

    public:
        RunningStats() :
            m_n(0) ,
            max_n(0),
            rs_complete(false)
            {}

        void Clear()
        {
            m_n = 0;
            rs_complete = false;
        }

        void setMaxCount(const size_t value)
        {
            max_n = value;
        }

        size_t getMaxCount() const
        {
            return max_n;
        }

        void Push(double x)
        {
            if(can_add())
            {
                m_n++;
                // See Knuth TAOCP vol 2, 3rd edition, page 232
                if (m_n == 1)
                {
                    m_oldM = m_newM = x;
                    m_oldS = 0.0;
                }
                else
                {
                    m_newM = m_oldM + (x - m_oldM)/m_n;
                    m_newS = m_oldS + (x - m_oldM)*(x - m_newM);

                    // set up for next iteration
                    m_oldM = m_newM;
                    m_oldS = m_newS;
                }
            }
        }

        size_t NumDataValues() const
        {
            return m_n;
        }

        double Mean() const
        {
            return (m_n> 0) ? m_newM : 0.0;
        }

        double Variance() const
        {
            return ( (m_n> 1) ? m_newS/(m_n - 1) : 0.0 );
        }

        double StandardDeviation() const
        {
            return sqrt( Variance() );
        }

        bool Full() const
        {
            return rs_complete;
        }

        bool NotFull() const
        {
            return !Full();
        }

        /*
            get/set the current settings, so that you can prime the running-stat with
            some known values
        */
        void getCurrentState(size_t& m_n_ret, double& m_oldM_ret, double& m_oldS_ret)
        {
            m_n_ret = m_n;
            m_oldM_ret = m_oldM;
            m_oldS_ret = m_oldS;
        }
        void setInitialValues(size_t m_n_init, double m_oldM_init, double m_oldS_init)
        {
            m_n = m_n_init;
            m_oldM = m_oldM_init;
            m_oldS = m_oldS_init;
        }



    private:
        size_t m_n;
        double m_oldM, m_newM, m_oldS, m_newS;
        size_t max_n;
        bool rs_complete;

        bool can_add()
        {
            if(max_n == 0)
            {
                return true;
            }
            if(rs_complete)
            {
                return false;
            }
            if(m_n == max_n)
            {
                rs_complete = true;
                return false;
            }
            return true;
        }

};
#endif // _RUNNING_STATS_H_