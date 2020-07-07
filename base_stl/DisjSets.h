//
// Created by 胡凌瑞 on 2020/7/7.
//

#ifndef DATA_STRUCTURE_CPP_DISJSETS_H
#define DATA_STRUCTURE_CPP_DISJSETS_H

#include <vector>
using std::vector;

/**
 * Disjoint set class.
 * Use union by rank and path compression.
 * Elements in the set are numbered starting at 0.
 */
class DisjSets
{
public:
    explicit DisjSets( int numElements ) : s(numElements, -1)
    {}

    int find( int x ) const
    {
        if( s[ x ] < 0 )
            return x;
        else
            return find( s[ x ] );
    }
    int find( int x )
    {
        if( s[ x ] < 0 )
            return x;
        else
            return s[ x ] = find( s[ x ] );
    }
    void unionSets( int root1, int root2 )
    {
        if( s[ root2 ] < s[ root1 ] )  // root2 is deeper
            s[ root1 ] = root2;        // Make root2 new root
        else
        {
            if( s[ root1 ] == s[ root2 ] )
                --s[ root1 ];          // Update height if same
            s[ root2 ] = root1;        // Make root1 new root
        }
    }

private:
    vector<int> s;
};

#endif //DATA_STRUCTURE_CPP_DISJSETS_H
