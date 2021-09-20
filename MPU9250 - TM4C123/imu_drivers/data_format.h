// -*-c++-*-

/*!
  \file data_format.h
  \brief
 */

#ifndef __IMU_BOARD_DATA_FORMAT_H__
#define __IMU_BOARD_DATA_FORMAT_H__

/*!

*/

template<class L, class R>
class Either {
private:

    template<class L, class R> friend Either<L, R> Left(L t_data);
    template<class L, class R> friend Either<L, R> Right(R t_data);

    union {
        L m_left;
        R m_right;
    };

    bool m_is_left;

public:

    bool left(L& t_data) {
        if (m_is_left) t_data = m_left;
        return m_is_left;
    }

    bool right(R& t_data) {
        if (!m_is_left) t_data = m_right;
        return !m_is_left;
    }
};

/*!

*/
template <class L, class R>
Either<L, R> Left(L t_data) {
    Either<L, R> ret_val;
    ret_val.m_is_left = true;
    ret_val.m_left = t_data;
    return ret_val;
}

/*!

*/
template <class L, class R>
Either<L, R> Right(R t_data) {
    Either<L, R> ret_val;
    ret_val.m_is_left = false;
    ret_val.m_right = t_data;
    return ret_val;
}

/*!
 \struct RawData
 \brief
*/

struct RawData {

    Either<int,uint16_t> x;
    Either<int,uint16_t> y;
    Either<int,uint16_t> z;

};

static Either<int,uint16_t> raw_valid(const uint16_t& t_value) {
    return Right<int,uint16_t>(t_value);
}

static Either<int,uint16_t> raw_invalid(const int& t_value) {
    return Left<int,uint16_t>(t_value);
}

/*!
 \struct FloatData
 \brief
*/

struct FloatData {

    Either<int,float> x;
    Either<int,float> y;
    Either<int,float> z;

};

static Either<int,float> float_valid(const float& t_value) {
    return Right<int,float>(t_value);
}

static Either<int,float> float_invalid(const int& t_value) {
    return Left<int,float>(t_value);
}

#endif
