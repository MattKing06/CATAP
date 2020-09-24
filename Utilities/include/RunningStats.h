//#ifndef _RUNNING_STATS_H_
//#define _RUNNING_STATS_H_
//
//#include <iostream>
//#include "PythonTypeConversions.h"
//
//class RunningStats
//{
//    // thanks to https://www.johndcook.com/blog/standard_deviation/
//
//    public:
//        RunningStats() :
//            m_n(0),
//            max_n(0),
//            rs_complete(false)
//            {}
//        /*! Clear the running stats values */
//        void Clear()
//        {
//            m_n = 0;
//            rs_complete = false;
//        }
//        /*! Set maximum number of entires to be added to running stats 
//            @param[in] number of entries         */
//        void setMaxCount(const size_t value)
//        {
//            max_n = value;
//        }
//        /*! Get the maximum number of entires 
//            @param[out] max number of entries         */
//        size_t getMaxCount() const
//        {
//            return max_n;
//        }
//        /*! Add a new value to the runing stats , templated version
//            @param[in] value to add, !!must be a umerica simple type!! */
//        template<typename T>
//        void Push(T x)
//        {
//            doPush((double)x);
//        }
//
//        /*! Add a new value to the runing stats , templated version
//            @param[in] value to add, !!must be a umerica simple type!! */
//        size_t NumDataValues() const
//        {
//            return m_n;
//        }
//        /*! Add a new value to the runing stats , templated version
//            @param[in] value to add, !!must be a umerica simple type!! */
//        double Mean() const
//        {
//            return (m_n> 0) ? m_newM : 0.0;
//        }
//        /*! Add a new value to the runing stats , templated version
//            @param[in] value to add, !!must be a umerica simple type!! */
//        double Variance() const
//        {
//            return ( (m_n> 1) ? m_newS/(m_n - 1) : 0.0 );
//        }
//        /*! Add a new value to the runing stats , templated version
//            @param[in] value to add, !!must be a umerica simple type!! */
//        double StandardDeviation() const
//        {
//            return sqrt( Variance() );
//        }
//        /*! Add a new value to the runing stats , templated version
//            @param[in] value to add, !!must be a umerica simple type!! */
//        bool Full() const
//        {
//            return rs_complete;
//        }
//        /*! Add a new value to the runing stats , templated version
//            @param[in] value to add, !!must be a umerica simple type!! */
//        bool NotFull() const
//        {
//            return !Full();
//        }
//        /*! updat epassed referecnes with teh latest value 
//            @param[in] value to add, !!must be a umerica simple type!! 
//            @param[in] value to add, !!must be a umerica simple type!! 
//            @param[in] value to add, !!must be a umerica simple type!! */
//        void getCurrentState(size_t& m_n_ret, double& m_oldM_ret, double& m_oldS_ret)
//        {
//            m_n_ret = m_n;
//            m_oldM_ret = m_oldM;
//            m_oldS_ret = m_oldS;
//        }
//        void setInitialValues(size_t m_n_init, double m_oldM_init, double m_oldS_init)
//        {
//            m_n = m_n_init;
//            m_oldM = m_oldM_init;
//            m_oldS = m_oldS_init;
//        }
//
//
//        std::vector<double> Buffer()
//        {
//            return buffer;
//        }
//        boost::python::list Buffer_Py()
//        {
//            return to_py_list<double>(buffer);
//        }
//
//        /*
//          get/set the current settings, so that you can prime the running-stat with
//            some known values
//        */
//        boost::python::dict getRunningStats()
//        {
//            boost::python::dict r;
//            r["NumDataValues"] = NumDataValues();
//            r["Mean"] = Mean();
//            r["Variance"] = Variance();
//            r["StandardDeviation"] = StandardDeviation();
//            r["Full"] = Full();
//            r["Buffer"] = Buffer_Py;
//            return r;
//        }
//
//        size_t setBufferSize(size_t s)
//        {
//            clearBuffer();
//            buffer.resize(s);
//        }
//
//        void clearBuffer()
//        {
//            buffer.clear();
//            buffer_n = 0;//MAGIC
//        }
//
//    private:
//        size_t m_n;
//        size_t buffer_n;
//        double m_oldM, m_newM, m_oldS, m_newS;
//        size_t max_n;
//        bool rs_complete;
//
//        bool can_add()
//        {
//            if(max_n == 0)
//            {
//                return true;
//            }
//            if(rs_complete)
//            {
//                return false;
//            }
//            if(m_n == max_n)
//            {
//                rs_complete = true;
//                return false;
//            }
//            return true;
//        }
//
//        std::vector<double> buffer;
//
//        void bufferPush(double value)
//        {
//            buffer[buffer_n] = value;
//            buffer_n += 1; //MAGIC
//        }
//
//
//        /*! Add a new value to the runing stats, assuming passed value is double
//        @param[in] value to add */
//        void doPush(double x)
//        {
//            if (can_add())
//            {
//                m_n++;
//                // See Knuth TAOCP vol 2, 3rd edition, page 232
//                if (m_n == 1)
//                {
//                    m_oldM = m_newM = x;
//                    m_oldS = 0.0;
//                }
//                else
//                {
//                    m_newM = m_oldM + (x - m_oldM) / m_n;
//                    m_newS = m_oldS + (x - m_oldM) * (x - m_newM);
//                    // set up for next iteration
//                    m_oldM = m_newM;
//                    m_oldS = m_newS;
//                }
//            }
//            bufferPush(x);
//        }
//
//};
//#endif // _RUNNING_STATS_H_