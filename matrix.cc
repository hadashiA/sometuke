#include "matrix.h"

namespace kawaii {

float mat4_get(const float *mat, int row, int col) {
    return mat[row + 4*col];
}

void mat4_set(float *mat, int row, int col, float value) {
    mat[row + 4*col] = value;
}

void mat4_swap(float *pIn, int r1, int c1, int r2, int c2) {
    float tmp = mat4_get(pIn,r1,c1);
    mat4_set(pIn,r1,c1,mat4_get(pIn,r2,c2));
    mat4_set(pIn,r2,c2, tmp);
}

bool gaussj(float *a, float *b) {
    int i, icol = 0, irow = 0, j, k, l, ll, n = 4, m = 4;
    float big, dum, pivinv;
    int indxc[n];
    int indxr[n];
    int ipiv[n];

    for (j = 0; j < n; j++) {
        ipiv[j] = 0;
    }

    for (i = 0; i < n; i++) {
        big = 0.0f;
        for (j = 0; j < n; j++) {
            if (ipiv[j] != 1) {
                for (k = 0; k < n; k++) {
                    if (ipiv[k] == 0) {
                        if (fabs(mat4_get(a,j, k)) >= big) {
                            big = fabs(mat4_get(a,j, k));
                            irow = j;
                            icol = k;
                        }
                    }
                }
            }
        }
        ++(ipiv[icol]);
        if (irow != icol) {
            for (l = 0; l < n; l++) {
                mat4_swap(a,irow, l, icol, l);
            }
            for (l = 0; l < m; l++) {
                mat4_swap(b,irow, l, icol, l);
            }
        }
        indxr[i] = irow;
        indxc[i] = icol;
        if (mat4_get(a,icol, icol) == 0.0) {
            return true;
        }
        pivinv = 1.0f / mat4_get(a,icol, icol);
        mat4_set(a,icol, icol, 1.0f);
        for (l = 0; l < n; l++) {
            mat4_set(a,icol, l, mat4_get(a,icol, l) * pivinv);
        }
        for (l = 0; l < m; l++) {
            mat4_set(b,icol, l, mat4_get(b,icol, l) * pivinv);
        }

        for (ll = 0; ll < n; ll++) {
            if (ll != icol) {
                dum = mat4_get(a,ll, icol);
                mat4_set(a,ll, icol, 0.0f);
                for (l = 0; l < n; l++) {
                    mat4_set(a,ll, l, mat4_get(a,ll, l) - mat4_get(a,icol, l) * dum);
                }
                for (l = 0; l < m; l++) {
                    mat4_set(b,ll, l, mat4_get(a,ll, l) - mat4_get(b,icol, l) * dum);
                }
            }
        }
    }
//    This is the end of the main loop over columns of the reduction. It only remains to unscram-
//    ble the solution in view of the column interchanges. We do this by interchanging pairs of
//    columns in the reverse order that the permutation was built up.
    for (l = n - 1; l >= 0; l--) {
        if (indxr[l] != indxc[l]) {
            for (k = 0; k < n; k++) {
                mat4_swap(a,k, indxr[l], k, indxc[l]);
            }
        }
    }
    return true;
}

Rect operator *(const Rect& rect, const mat4& m)  {
    vec4 pos1(rect.pos.x, rect.pos.y, 0, 1);
    vec4 pos2(rect.pos.x + rect.size.x, rect.pos.y + rect.size.y, 0, 1);

    vec4 new_pos1 = pos1 * m;
    vec4 new_pos2 = pos2 * m;
    vec2 new_size(new_pos2.x - new_pos1.x, new_pos2.y - new_pos1.y);

    return Rect(new_pos1.x, new_pos1.y, new_size.x, new_size.y);
}

}


