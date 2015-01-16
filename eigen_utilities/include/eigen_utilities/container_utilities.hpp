/**
 * @author Eric Cousineau <eacousineau@gmail.com>, member of Dr. Aaron
 * Ames's AMBER Lab
 */
#ifndef EIGEN_UTILITIES_CONTAINER_UTILITIES_H
    #define EIGEN_UTILITIES_CONTAINER_UTILITIES_H

#include <Eigen/Dense>

#include <eigen_utilities/assert_size.hpp>

namespace eigen_utilities
{

template<typename DerivedA, typename DerivedB, typename Container>
void select_rows(const Eigen::MatrixBase<DerivedA> &in, const Container &indices, Eigen::MatrixBase<DerivedB> &out)
{
    assert_size(out, indices.size(), in.cols());
    for (int i = 0; i < indices.size(); ++i)
        out.row(i) = in.row(indices[i]);
}

template<typename DerivedA, typename DerivedB, typename Container>
void select_rows_resize(const Eigen::MatrixBase<DerivedA> &in, const Container &indices, Eigen::MatrixBase<DerivedB> &out)
{
    out.derived().resize(indices.size(), in.cols());
    select_rows(in, indices, out);
}

template<typename DerivedA, typename DerivedB, typename Container>
void select_cols(const Eigen::MatrixBase<DerivedA> &in, const Container &indices, Eigen::MatrixBase<DerivedB> &out)
{
    assert_size(out, in.rows(), indices.size());
    for (int i = 0; i < indices.size(); ++i)
        out.col(i) = in.col(indices[i]);
}

template<typename DerivedA, typename DerivedB, typename Container>
void select_cols_resize(const Eigen::MatrixBase<DerivedA> &in, const Container &indices, Eigen::MatrixBase<DerivedB> &out)
{
    out.derived().resize(in.rows(), indices.size());
    select_cols(in, indices, out);
}

template<typename DerivedA, typename DerivedB, typename ContainerA, typename ContainerB>
void select_grid(const Eigen::MatrixBase<DerivedA> &in, const ContainerA &rows, const ContainerB &cols, Eigen::MatrixBase<DerivedB> &out)
{
    assert_size(out, rows.size(), cols.size());
    for (int i = 0; i < rows.size(); ++i)
    {
        for (int j = 0; j < cols.size(); ++j)
            out(i, j) = in(rows[i], cols[j]);
    }
}

template<typename DerivedA, typename DerivedB, typename ContainerA, typename ContainerB>
void select_grid_resize(const Eigen::MatrixBase<DerivedA> &in, const ContainerA &rows, const ContainerB &cols, Eigen::MatrixBase<DerivedB> &out)
{
    out.derived().resize(rows.size(), cols.size());
    select_grid(in, rows, cols, out);
}


template<typename DerivedA, typename DerivedB, typename Container>
void select_rows_reverse(const Eigen::MatrixBase<DerivedA> &in, const Container &indices, Eigen::MatrixBase<DerivedB> &out)
{
    assert_size(in, indices.size(), in.cols());
    for (int i = 0; i < indices.size(); ++i)
        out.row(indices[i]) = in.row(i);
}

template<typename DerivedA, typename DerivedB, typename Container>
void select_cols_reverse(const Eigen::MatrixBase<DerivedA> &in, const Container &indices, Eigen::MatrixBase<DerivedB> &out)
{
    assert_size(in, in.rows(), indices.size());
    for (int i = 0; i < indices.size(); ++i)
        out.col(indices[i]) = in.col(i);
}

}

#endif // EIGEN_UTILITIES_CONTAINER_UTILITIES_H
