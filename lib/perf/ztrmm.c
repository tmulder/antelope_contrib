#include "blaswrap.h"
#include "f2c.h"

/* Subroutine */ int ztrmm_(char *side, char *uplo, char *transa, char *diag, 
	integer *m, integer *n, doublecomplex *alpha, doublecomplex *a, 
	integer *lda, doublecomplex *b, integer *ldb)
{
    /* System generated locals */
    integer a_dim1, a_offset, b_dim1, b_offset, i__1, i__2, i__3, i__4, i__5, 
	    i__6;
    doublecomplex z__1, z__2, z__3;
    /* Builtin functions */
    void d_cnjg(doublecomplex *, doublecomplex *);
    /* Local variables */
    static integer info;
    static doublecomplex temp;
    static integer i__, j, k;
    static logical lside;
    extern logical lsame_(char *, char *);
    static integer nrowa;
    static logical upper;
    extern /* Subroutine */ int xerbla_(char *, integer *);
    static logical noconj, nounit;
#define a_subscr(a_1,a_2) (a_2)*a_dim1 + a_1
#define a_ref(a_1,a_2) a[a_subscr(a_1,a_2)]
#define b_subscr(a_1,a_2) (a_2)*b_dim1 + a_1
#define b_ref(a_1,a_2) b[b_subscr(a_1,a_2)]
/*  Purpose   
    =======   
    ZTRMM  performs one of the matrix-matrix operations   
       B := alpha*op( A )*B,   or   B := alpha*B*op( A )   
    where  alpha  is a scalar,  B  is an m by n matrix,  A  is a unit, or   
    non-unit,  upper or lower triangular matrix  and  op( A )  is one  of   
       op( A ) = A   or   op( A ) = A'   or   op( A ) = conjg( A' ).   
    Parameters   
    ==========   
    SIDE   - CHARACTER*1.   
             On entry,  SIDE specifies whether  op( A ) multiplies B from   
             the left or right as follows:   
                SIDE = 'L' or 'l'   B := alpha*op( A )*B.   
                SIDE = 'R' or 'r'   B := alpha*B*op( A ).   
             Unchanged on exit.   
    UPLO   - CHARACTER*1.   
             On entry, UPLO specifies whether the matrix A is an upper or   
             lower triangular matrix as follows:   
                UPLO = 'U' or 'u'   A is an upper triangular matrix.   
                UPLO = 'L' or 'l'   A is a lower triangular matrix.   
             Unchanged on exit.   
    TRANSA - CHARACTER*1.   
             On entry, TRANSA specifies the form of op( A ) to be used in   
             the matrix multiplication as follows:   
                TRANSA = 'N' or 'n'   op( A ) = A.   
                TRANSA = 'T' or 't'   op( A ) = A'.   
                TRANSA = 'C' or 'c'   op( A ) = conjg( A' ).   
             Unchanged on exit.   
    DIAG   - CHARACTER*1.   
             On entry, DIAG specifies whether or not A is unit triangular   
             as follows:   
                DIAG = 'U' or 'u'   A is assumed to be unit triangular.   
                DIAG = 'N' or 'n'   A is not assumed to be unit   
                                    triangular.   
             Unchanged on exit.   
    M      - INTEGER.   
             On entry, M specifies the number of rows of B. M must be at   
             least zero.   
             Unchanged on exit.   
    N      - INTEGER.   
             On entry, N specifies the number of columns of B.  N must be   
             at least zero.   
             Unchanged on exit.   
    ALPHA  - COMPLEX*16      .   
             On entry,  ALPHA specifies the scalar  alpha. When  alpha is   
             zero then  A is not referenced and  B need not be set before   
             entry.   
             Unchanged on exit.   
    A      - COMPLEX*16       array of DIMENSION ( LDA, k ), where k is m   
             when  SIDE = 'L' or 'l'  and is  n  when  SIDE = 'R' or 'r'.   
             Before entry  with  UPLO = 'U' or 'u',  the  leading  k by k   
             upper triangular part of the array  A must contain the upper   
             triangular matrix  and the strictly lower triangular part of   
             A is not referenced.   
             Before entry  with  UPLO = 'L' or 'l',  the  leading  k by k   
             lower triangular part of the array  A must contain the lower   
             triangular matrix  and the strictly upper triangular part of   
             A is not referenced.   
             Note that when  DIAG = 'U' or 'u',  the diagonal elements of   
             A  are not referenced either,  but are assumed to be  unity.   
             Unchanged on exit.   
    LDA    - INTEGER.   
             On entry, LDA specifies the first dimension of A as declared   
             in the calling (sub) program.  When  SIDE = 'L' or 'l'  then   
             LDA  must be at least  max( 1, m ),  when  SIDE = 'R' or 'r'   
             then LDA must be at least max( 1, n ).   
             Unchanged on exit.   
    B      - COMPLEX*16       array of DIMENSION ( LDB, n ).   
             Before entry,  the leading  m by n part of the array  B must   
             contain the matrix  B,  and  on exit  is overwritten  by the   
             transformed matrix.   
    LDB    - INTEGER.   
             On entry, LDB specifies the first dimension of B as declared   
             in  the  calling  (sub)  program.   LDB  must  be  at  least   
             max( 1, m ).   
             Unchanged on exit.   
    Level 3 Blas routine.   
    -- Written on 8-February-1989.   
       Jack Dongarra, Argonne National Laboratory.   
       Iain Duff, AERE Harwell.   
       Jeremy Du Croz, Numerical Algorithms Group Ltd.   
       Sven Hammarling, Numerical Algorithms Group Ltd.   
       Test the input parameters.   
       Parameter adjustments */
    a_dim1 = *lda;
    a_offset = 1 + a_dim1 * 1;
    a -= a_offset;
    b_dim1 = *ldb;
    b_offset = 1 + b_dim1 * 1;
    b -= b_offset;
    /* Function Body */
    lside = lsame_(side, "L");
    if (lside) {
	nrowa = *m;
    } else {
	nrowa = *n;
    }
    noconj = lsame_(transa, "T");
    nounit = lsame_(diag, "N");
    upper = lsame_(uplo, "U");
    info = 0;
    if (! lside && ! lsame_(side, "R")) {
	info = 1;
    } else if (! upper && ! lsame_(uplo, "L")) {
	info = 2;
    } else if (! lsame_(transa, "N") && ! lsame_(transa,
	     "T") && ! lsame_(transa, "C")) {
	info = 3;
    } else if (! lsame_(diag, "U") && ! lsame_(diag, 
	    "N")) {
	info = 4;
    } else if (*m < 0) {
	info = 5;
    } else if (*n < 0) {
	info = 6;
    } else if (*lda < max(1,nrowa)) {
	info = 9;
    } else if (*ldb < max(1,*m)) {
	info = 11;
    }
    if (info != 0) {
	xerbla_("ZTRMM ", &info);
	return 0;
    }
/*     Quick return if possible. */
    if (*n == 0) {
	return 0;
    }
/*     And when  alpha.eq.zero. */
    if (alpha->r == 0. && alpha->i == 0.) {
	i__1 = *n;
	for (j = 1; j <= i__1; ++j) {
	    i__2 = *m;
	    for (i__ = 1; i__ <= i__2; ++i__) {
		i__3 = b_subscr(i__, j);
		b[i__3].r = 0., b[i__3].i = 0.;
/* L10: */
	    }
/* L20: */
	}
	return 0;
    }
/*     Start the operations. */
    if (lside) {
	if (lsame_(transa, "N")) {
/*           Form  B := alpha*A*B. */
	    if (upper) {
		i__1 = *n;
		for (j = 1; j <= i__1; ++j) {
		    i__2 = *m;
		    for (k = 1; k <= i__2; ++k) {
			i__3 = b_subscr(k, j);
			if (b[i__3].r != 0. || b[i__3].i != 0.) {
			    i__3 = b_subscr(k, j);
			    z__1.r = alpha->r * b[i__3].r - alpha->i * b[i__3]
				    .i, z__1.i = alpha->r * b[i__3].i + 
				    alpha->i * b[i__3].r;
			    temp.r = z__1.r, temp.i = z__1.i;
			    i__3 = k - 1;
			    for (i__ = 1; i__ <= i__3; ++i__) {
				i__4 = b_subscr(i__, j);
				i__5 = b_subscr(i__, j);
				i__6 = a_subscr(i__, k);
				z__2.r = temp.r * a[i__6].r - temp.i * a[i__6]
					.i, z__2.i = temp.r * a[i__6].i + 
					temp.i * a[i__6].r;
				z__1.r = b[i__5].r + z__2.r, z__1.i = b[i__5]
					.i + z__2.i;
				b[i__4].r = z__1.r, b[i__4].i = z__1.i;
/* L30: */
			    }
			    if (nounit) {
				i__3 = a_subscr(k, k);
				z__1.r = temp.r * a[i__3].r - temp.i * a[i__3]
					.i, z__1.i = temp.r * a[i__3].i + 
					temp.i * a[i__3].r;
				temp.r = z__1.r, temp.i = z__1.i;
			    }
			    i__3 = b_subscr(k, j);
			    b[i__3].r = temp.r, b[i__3].i = temp.i;
			}
/* L40: */
		    }
/* L50: */
		}
	    } else {
		i__1 = *n;
		for (j = 1; j <= i__1; ++j) {
		    for (k = *m; k >= 1; --k) {
			i__2 = b_subscr(k, j);
			if (b[i__2].r != 0. || b[i__2].i != 0.) {
			    i__2 = b_subscr(k, j);
			    z__1.r = alpha->r * b[i__2].r - alpha->i * b[i__2]
				    .i, z__1.i = alpha->r * b[i__2].i + 
				    alpha->i * b[i__2].r;
			    temp.r = z__1.r, temp.i = z__1.i;
			    i__2 = b_subscr(k, j);
			    b[i__2].r = temp.r, b[i__2].i = temp.i;
			    if (nounit) {
				i__2 = b_subscr(k, j);
				i__3 = b_subscr(k, j);
				i__4 = a_subscr(k, k);
				z__1.r = b[i__3].r * a[i__4].r - b[i__3].i * 
					a[i__4].i, z__1.i = b[i__3].r * a[
					i__4].i + b[i__3].i * a[i__4].r;
				b[i__2].r = z__1.r, b[i__2].i = z__1.i;
			    }
			    i__2 = *m;
			    for (i__ = k + 1; i__ <= i__2; ++i__) {
				i__3 = b_subscr(i__, j);
				i__4 = b_subscr(i__, j);
				i__5 = a_subscr(i__, k);
				z__2.r = temp.r * a[i__5].r - temp.i * a[i__5]
					.i, z__2.i = temp.r * a[i__5].i + 
					temp.i * a[i__5].r;
				z__1.r = b[i__4].r + z__2.r, z__1.i = b[i__4]
					.i + z__2.i;
				b[i__3].r = z__1.r, b[i__3].i = z__1.i;
/* L60: */
			    }
			}
/* L70: */
		    }
/* L80: */
		}
	    }
	} else {
/*           Form  B := alpha*A'*B   or   B := alpha*conjg( A' )*B. */
	    if (upper) {
		i__1 = *n;
		for (j = 1; j <= i__1; ++j) {
		    for (i__ = *m; i__ >= 1; --i__) {
			i__2 = b_subscr(i__, j);
			temp.r = b[i__2].r, temp.i = b[i__2].i;
			if (noconj) {
			    if (nounit) {
				i__2 = a_subscr(i__, i__);
				z__1.r = temp.r * a[i__2].r - temp.i * a[i__2]
					.i, z__1.i = temp.r * a[i__2].i + 
					temp.i * a[i__2].r;
				temp.r = z__1.r, temp.i = z__1.i;
			    }
			    i__2 = i__ - 1;
			    for (k = 1; k <= i__2; ++k) {
				i__3 = a_subscr(k, i__);
				i__4 = b_subscr(k, j);
				z__2.r = a[i__3].r * b[i__4].r - a[i__3].i * 
					b[i__4].i, z__2.i = a[i__3].r * b[
					i__4].i + a[i__3].i * b[i__4].r;
				z__1.r = temp.r + z__2.r, z__1.i = temp.i + 
					z__2.i;
				temp.r = z__1.r, temp.i = z__1.i;
/* L90: */
			    }
			} else {
			    if (nounit) {
				d_cnjg(&z__2, &a_ref(i__, i__));
				z__1.r = temp.r * z__2.r - temp.i * z__2.i, 
					z__1.i = temp.r * z__2.i + temp.i * 
					z__2.r;
				temp.r = z__1.r, temp.i = z__1.i;
			    }
			    i__2 = i__ - 1;
			    for (k = 1; k <= i__2; ++k) {
				d_cnjg(&z__3, &a_ref(k, i__));
				i__3 = b_subscr(k, j);
				z__2.r = z__3.r * b[i__3].r - z__3.i * b[i__3]
					.i, z__2.i = z__3.r * b[i__3].i + 
					z__3.i * b[i__3].r;
				z__1.r = temp.r + z__2.r, z__1.i = temp.i + 
					z__2.i;
				temp.r = z__1.r, temp.i = z__1.i;
/* L100: */
			    }
			}
			i__2 = b_subscr(i__, j);
			z__1.r = alpha->r * temp.r - alpha->i * temp.i, 
				z__1.i = alpha->r * temp.i + alpha->i * 
				temp.r;
			b[i__2].r = z__1.r, b[i__2].i = z__1.i;
/* L110: */
		    }
/* L120: */
		}
	    } else {
		i__1 = *n;
		for (j = 1; j <= i__1; ++j) {
		    i__2 = *m;
		    for (i__ = 1; i__ <= i__2; ++i__) {
			i__3 = b_subscr(i__, j);
			temp.r = b[i__3].r, temp.i = b[i__3].i;
			if (noconj) {
			    if (nounit) {
				i__3 = a_subscr(i__, i__);
				z__1.r = temp.r * a[i__3].r - temp.i * a[i__3]
					.i, z__1.i = temp.r * a[i__3].i + 
					temp.i * a[i__3].r;
				temp.r = z__1.r, temp.i = z__1.i;
			    }
			    i__3 = *m;
			    for (k = i__ + 1; k <= i__3; ++k) {
				i__4 = a_subscr(k, i__);
				i__5 = b_subscr(k, j);
				z__2.r = a[i__4].r * b[i__5].r - a[i__4].i * 
					b[i__5].i, z__2.i = a[i__4].r * b[
					i__5].i + a[i__4].i * b[i__5].r;
				z__1.r = temp.r + z__2.r, z__1.i = temp.i + 
					z__2.i;
				temp.r = z__1.r, temp.i = z__1.i;
/* L130: */
			    }
			} else {
			    if (nounit) {
				d_cnjg(&z__2, &a_ref(i__, i__));
				z__1.r = temp.r * z__2.r - temp.i * z__2.i, 
					z__1.i = temp.r * z__2.i + temp.i * 
					z__2.r;
				temp.r = z__1.r, temp.i = z__1.i;
			    }
			    i__3 = *m;
			    for (k = i__ + 1; k <= i__3; ++k) {
				d_cnjg(&z__3, &a_ref(k, i__));
				i__4 = b_subscr(k, j);
				z__2.r = z__3.r * b[i__4].r - z__3.i * b[i__4]
					.i, z__2.i = z__3.r * b[i__4].i + 
					z__3.i * b[i__4].r;
				z__1.r = temp.r + z__2.r, z__1.i = temp.i + 
					z__2.i;
				temp.r = z__1.r, temp.i = z__1.i;
/* L140: */
			    }
			}
			i__3 = b_subscr(i__, j);
			z__1.r = alpha->r * temp.r - alpha->i * temp.i, 
				z__1.i = alpha->r * temp.i + alpha->i * 
				temp.r;
			b[i__3].r = z__1.r, b[i__3].i = z__1.i;
/* L150: */
		    }
/* L160: */
		}
	    }
	}
    } else {
	if (lsame_(transa, "N")) {
/*           Form  B := alpha*B*A. */
	    if (upper) {
		for (j = *n; j >= 1; --j) {
		    temp.r = alpha->r, temp.i = alpha->i;
		    if (nounit) {
			i__1 = a_subscr(j, j);
			z__1.r = temp.r * a[i__1].r - temp.i * a[i__1].i, 
				z__1.i = temp.r * a[i__1].i + temp.i * a[i__1]
				.r;
			temp.r = z__1.r, temp.i = z__1.i;
		    }
		    i__1 = *m;
		    for (i__ = 1; i__ <= i__1; ++i__) {
			i__2 = b_subscr(i__, j);
			i__3 = b_subscr(i__, j);
			z__1.r = temp.r * b[i__3].r - temp.i * b[i__3].i, 
				z__1.i = temp.r * b[i__3].i + temp.i * b[i__3]
				.r;
			b[i__2].r = z__1.r, b[i__2].i = z__1.i;
/* L170: */
		    }
		    i__1 = j - 1;
		    for (k = 1; k <= i__1; ++k) {
			i__2 = a_subscr(k, j);
			if (a[i__2].r != 0. || a[i__2].i != 0.) {
			    i__2 = a_subscr(k, j);
			    z__1.r = alpha->r * a[i__2].r - alpha->i * a[i__2]
				    .i, z__1.i = alpha->r * a[i__2].i + 
				    alpha->i * a[i__2].r;
			    temp.r = z__1.r, temp.i = z__1.i;
			    i__2 = *m;
			    for (i__ = 1; i__ <= i__2; ++i__) {
				i__3 = b_subscr(i__, j);
				i__4 = b_subscr(i__, j);
				i__5 = b_subscr(i__, k);
				z__2.r = temp.r * b[i__5].r - temp.i * b[i__5]
					.i, z__2.i = temp.r * b[i__5].i + 
					temp.i * b[i__5].r;
				z__1.r = b[i__4].r + z__2.r, z__1.i = b[i__4]
					.i + z__2.i;
				b[i__3].r = z__1.r, b[i__3].i = z__1.i;
/* L180: */
			    }
			}
/* L190: */
		    }
/* L200: */
		}
	    } else {
		i__1 = *n;
		for (j = 1; j <= i__1; ++j) {
		    temp.r = alpha->r, temp.i = alpha->i;
		    if (nounit) {
			i__2 = a_subscr(j, j);
			z__1.r = temp.r * a[i__2].r - temp.i * a[i__2].i, 
				z__1.i = temp.r * a[i__2].i + temp.i * a[i__2]
				.r;
			temp.r = z__1.r, temp.i = z__1.i;
		    }
		    i__2 = *m;
		    for (i__ = 1; i__ <= i__2; ++i__) {
			i__3 = b_subscr(i__, j);
			i__4 = b_subscr(i__, j);
			z__1.r = temp.r * b[i__4].r - temp.i * b[i__4].i, 
				z__1.i = temp.r * b[i__4].i + temp.i * b[i__4]
				.r;
			b[i__3].r = z__1.r, b[i__3].i = z__1.i;
/* L210: */
		    }
		    i__2 = *n;
		    for (k = j + 1; k <= i__2; ++k) {
			i__3 = a_subscr(k, j);
			if (a[i__3].r != 0. || a[i__3].i != 0.) {
			    i__3 = a_subscr(k, j);
			    z__1.r = alpha->r * a[i__3].r - alpha->i * a[i__3]
				    .i, z__1.i = alpha->r * a[i__3].i + 
				    alpha->i * a[i__3].r;
			    temp.r = z__1.r, temp.i = z__1.i;
			    i__3 = *m;
			    for (i__ = 1; i__ <= i__3; ++i__) {
				i__4 = b_subscr(i__, j);
				i__5 = b_subscr(i__, j);
				i__6 = b_subscr(i__, k);
				z__2.r = temp.r * b[i__6].r - temp.i * b[i__6]
					.i, z__2.i = temp.r * b[i__6].i + 
					temp.i * b[i__6].r;
				z__1.r = b[i__5].r + z__2.r, z__1.i = b[i__5]
					.i + z__2.i;
				b[i__4].r = z__1.r, b[i__4].i = z__1.i;
/* L220: */
			    }
			}
/* L230: */
		    }
/* L240: */
		}
	    }
	} else {
/*           Form  B := alpha*B*A'   or   B := alpha*B*conjg( A' ). */
	    if (upper) {
		i__1 = *n;
		for (k = 1; k <= i__1; ++k) {
		    i__2 = k - 1;
		    for (j = 1; j <= i__2; ++j) {
			i__3 = a_subscr(j, k);
			if (a[i__3].r != 0. || a[i__3].i != 0.) {
			    if (noconj) {
				i__3 = a_subscr(j, k);
				z__1.r = alpha->r * a[i__3].r - alpha->i * a[
					i__3].i, z__1.i = alpha->r * a[i__3]
					.i + alpha->i * a[i__3].r;
				temp.r = z__1.r, temp.i = z__1.i;
			    } else {
				d_cnjg(&z__2, &a_ref(j, k));
				z__1.r = alpha->r * z__2.r - alpha->i * 
					z__2.i, z__1.i = alpha->r * z__2.i + 
					alpha->i * z__2.r;
				temp.r = z__1.r, temp.i = z__1.i;
			    }
			    i__3 = *m;
			    for (i__ = 1; i__ <= i__3; ++i__) {
				i__4 = b_subscr(i__, j);
				i__5 = b_subscr(i__, j);
				i__6 = b_subscr(i__, k);
				z__2.r = temp.r * b[i__6].r - temp.i * b[i__6]
					.i, z__2.i = temp.r * b[i__6].i + 
					temp.i * b[i__6].r;
				z__1.r = b[i__5].r + z__2.r, z__1.i = b[i__5]
					.i + z__2.i;
				b[i__4].r = z__1.r, b[i__4].i = z__1.i;
/* L250: */
			    }
			}
/* L260: */
		    }
		    temp.r = alpha->r, temp.i = alpha->i;
		    if (nounit) {
			if (noconj) {
			    i__2 = a_subscr(k, k);
			    z__1.r = temp.r * a[i__2].r - temp.i * a[i__2].i, 
				    z__1.i = temp.r * a[i__2].i + temp.i * a[
				    i__2].r;
			    temp.r = z__1.r, temp.i = z__1.i;
			} else {
			    d_cnjg(&z__2, &a_ref(k, k));
			    z__1.r = temp.r * z__2.r - temp.i * z__2.i, 
				    z__1.i = temp.r * z__2.i + temp.i * 
				    z__2.r;
			    temp.r = z__1.r, temp.i = z__1.i;
			}
		    }
		    if (temp.r != 1. || temp.i != 0.) {
			i__2 = *m;
			for (i__ = 1; i__ <= i__2; ++i__) {
			    i__3 = b_subscr(i__, k);
			    i__4 = b_subscr(i__, k);
			    z__1.r = temp.r * b[i__4].r - temp.i * b[i__4].i, 
				    z__1.i = temp.r * b[i__4].i + temp.i * b[
				    i__4].r;
			    b[i__3].r = z__1.r, b[i__3].i = z__1.i;
/* L270: */
			}
		    }
/* L280: */
		}
	    } else {
		for (k = *n; k >= 1; --k) {
		    i__1 = *n;
		    for (j = k + 1; j <= i__1; ++j) {
			i__2 = a_subscr(j, k);
			if (a[i__2].r != 0. || a[i__2].i != 0.) {
			    if (noconj) {
				i__2 = a_subscr(j, k);
				z__1.r = alpha->r * a[i__2].r - alpha->i * a[
					i__2].i, z__1.i = alpha->r * a[i__2]
					.i + alpha->i * a[i__2].r;
				temp.r = z__1.r, temp.i = z__1.i;
			    } else {
				d_cnjg(&z__2, &a_ref(j, k));
				z__1.r = alpha->r * z__2.r - alpha->i * 
					z__2.i, z__1.i = alpha->r * z__2.i + 
					alpha->i * z__2.r;
				temp.r = z__1.r, temp.i = z__1.i;
			    }
			    i__2 = *m;
			    for (i__ = 1; i__ <= i__2; ++i__) {
				i__3 = b_subscr(i__, j);
				i__4 = b_subscr(i__, j);
				i__5 = b_subscr(i__, k);
				z__2.r = temp.r * b[i__5].r - temp.i * b[i__5]
					.i, z__2.i = temp.r * b[i__5].i + 
					temp.i * b[i__5].r;
				z__1.r = b[i__4].r + z__2.r, z__1.i = b[i__4]
					.i + z__2.i;
				b[i__3].r = z__1.r, b[i__3].i = z__1.i;
/* L290: */
			    }
			}
/* L300: */
		    }
		    temp.r = alpha->r, temp.i = alpha->i;
		    if (nounit) {
			if (noconj) {
			    i__1 = a_subscr(k, k);
			    z__1.r = temp.r * a[i__1].r - temp.i * a[i__1].i, 
				    z__1.i = temp.r * a[i__1].i + temp.i * a[
				    i__1].r;
			    temp.r = z__1.r, temp.i = z__1.i;
			} else {
			    d_cnjg(&z__2, &a_ref(k, k));
			    z__1.r = temp.r * z__2.r - temp.i * z__2.i, 
				    z__1.i = temp.r * z__2.i + temp.i * 
				    z__2.r;
			    temp.r = z__1.r, temp.i = z__1.i;
			}
		    }
		    if (temp.r != 1. || temp.i != 0.) {
			i__1 = *m;
			for (i__ = 1; i__ <= i__1; ++i__) {
			    i__2 = b_subscr(i__, k);
			    i__3 = b_subscr(i__, k);
			    z__1.r = temp.r * b[i__3].r - temp.i * b[i__3].i, 
				    z__1.i = temp.r * b[i__3].i + temp.i * b[
				    i__3].r;
			    b[i__2].r = z__1.r, b[i__2].i = z__1.i;
/* L310: */
			}
		    }
/* L320: */
		}
	    }
	}
    }
    return 0;
/*     End of ZTRMM . */
} /* ztrmm_ */
#undef b_ref
#undef b_subscr
#undef a_ref
#undef a_subscr

