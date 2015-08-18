#ifndef COUNTER_HXX
#define COUNTER_HXX

#include <vector>

#include "int_array.hxx"

template<class IntArrayProvider=IntVectorProvider>
class ProfileCounter
{
public:
    template<class Matrix>
    int getComponentsCount(const Matrix &m) const
    {
        IntArrayProvider arrayProvider;

        int ht = m.getMatrixHeight();
        int wd = m.getMatrixWidth();
        long int numerator;
        auto vectorUp = arrayProvider.create(wd);
        int numUpperComponents;
        if ( m.getNumber( 0, 0 ) == 1 )
        {
            vectorUp[ 0 ] = 1;
            numerator = 1;
            numUpperComponents = 1;
        }
        else
        {
            vectorUp[ 0 ] = 0;
            numerator = 0;
            numUpperComponents = 0;
        }
        for ( int j = 1; j < wd; ++ j )
        {
            if (  m.getNumber( 0, j ) == 0 )
            {
                vectorUp[ j ] = 0;
            }
            else
            {
                if ( m.getNumber( 0, j - 1 ) == 1 )
                {
                    vectorUp[ j ] = numerator;
                }
                else
                {
                    ++numUpperComponents;
                    ++numerator;
                    vectorUp[ j ] = numerator;
                }
            }
        }

        auto vectorDown = arrayProvider.create(wd);
        for ( int i = 0; i < ( ht - 1 ); ++ i )
        {
            int up = i;
            int down = i + 1;
            // vector< int > vectorDown( wd );
            int numDownLineComponents = 0;
            if ( m.getNumber( i + 1, 0 ) == 1 )
            {
                ++ numerator;
                vectorDown[ 0 ] = numerator;
                ++ numDownLineComponents;
            }
            else
            {
                vectorDown[ 0 ] = 0;
            }
            for ( int j = 1; j < wd; ++ j )
            {
                if ( m.getNumber( i + 1, j ) == 0 )
                {
                    vectorDown[ j ] = 0;
                }
                else
                {
                    if ( m.getNumber( i + 1, j - 1 ) == 1 )
                    {
                        vectorDown[ j ] = numerator;
                    }
                    else
                    {
                        ++ numDownLineComponents;
                        ++ numerator;
                        vectorDown[ j ] = numerator;
                    }
                }           
            }
            int joining = 0;
            if ( m.getNumber( down, 0 ) == 1 && m.getNumber( up, 0 ) == 1 )//0-0
            {
                ++ joining;
                ++ numerator;
                int newnumerator = numerator;
                int upOldNumerator = vectorUp[ 0 ];
                int downOldNumerator = vectorDown[ 0 ];
                for ( int k = 0; k < wd; ++ k )
                {
                    if ( ( vectorUp[ k ] == upOldNumerator ) || ( vectorUp[ k ] == downOldNumerator ) )
                    {
                        vectorUp[ k ] = newnumerator;
                    }
                    if ( ( vectorDown[ k ] == downOldNumerator ) || ( vectorDown[ k ] == upOldNumerator ) )
                    {
                        vectorDown[ k ] = newnumerator;
                    }               
                }
            }//0-0

            if (wd > 1)
            {
                if ( ( m.getNumber( down, 0 ) == 1 &&  m.getNumber( up, 1 ) == 1 ) && ( vectorDown[ 0 ] != vectorUp[ 1 ]  ) )//0-1
                {
                    ++ joining;
                    ++ numerator;
                    int newnumerator = numerator;
                    int upOldNumerator = vectorUp[ 1 ];
                    int downOldNumerator = vectorDown[ 0 ];
                    for ( int k = 0; k < wd; ++ k )
                    {
                        if ( ( vectorUp[ k ] == upOldNumerator ) || ( vectorUp[ k ] == downOldNumerator ) )
                        {
                            vectorUp[ k ] = newnumerator;
                        }
                        if ( ( vectorDown[ k ] == downOldNumerator ) || ( vectorDown[ k ] == upOldNumerator ) )
                        {
                            vectorDown[ k ] = newnumerator;
                        }               
                    }
                }//0-1
                for ( int j = 1; j < wd - 1; ++ j )
                {
                    if ( ( m.getNumber( down, j ) == 1 &&  m.getNumber( up, j - 1 ) == 1 ) && ( vectorDown[ j ] != vectorUp[ j - 1 ]  ) )//j-j-1
                    {
                        ++ joining;
                        ++ numerator;
                        int newnumerator = numerator;
                        int upOldNumerator = vectorUp[ j - 1 ];
                        int downOldNumerator = vectorDown[ j ];
                        for ( int k = 0; k < wd; ++ k )
                        {
                            if ( ( vectorUp[ k ] == upOldNumerator ) || ( vectorUp[ k ] == downOldNumerator )  )
                            {
                                vectorUp[ k ] = newnumerator;
                            }
                            if ( ( vectorDown[ k ] == downOldNumerator ) || ( vectorDown[ k ] == upOldNumerator )  )
                            {
                                vectorDown[ k ] = newnumerator;
                            }               
                        }
                    }//j-j-1
                    if ( ( m.getNumber( down, j ) == 1 && m.getNumber( up, j ) == 1 ) && ( vectorDown[ j ] != vectorUp[ j ]  ) )//j-j
                    {
                        ++ joining;
                        ++ numerator;
                        int newnumerator = numerator;
                        int upOldNumerator = vectorUp[ j ];
                        int downOldNumerator = vectorDown[ j ];
                        for ( int k = 0; k < wd; ++ k )
                        {
                            if ( ( vectorUp[ k ] == upOldNumerator ) || ( vectorUp[ k ] == downOldNumerator ) )
                            {
                                vectorUp[ k ] = newnumerator;
                            }
                            if ( ( vectorDown[ k ] == downOldNumerator ) || ( vectorDown[ k ] == upOldNumerator ) )
                            {
                                vectorDown[ k ] = newnumerator;
                            }               
                        }
                    }//j-j
                    if ( ( m.getNumber( down, j ) == 1 && m.getNumber( up, j + 1 ) == 1 ) && ( vectorDown[ j ] != vectorUp[ j + 1 ]  ) )//j-j+1
                    {
                        ++ joining;
                        ++ numerator;
                        int newnumerator = numerator;
                        int upOldNumerator = vectorUp[ j + 1 ];
                        int downOldNumerator = vectorDown[ j ];
                        for ( int k = 0; k < wd; ++ k )
                        {
                            if ( ( vectorUp[ k ] == upOldNumerator ) || ( vectorUp[ k ] == downOldNumerator ) )
                            {
                                vectorUp[ k ] = newnumerator;
                            }
                            if ( ( vectorDown[ k ] == downOldNumerator ) || ( vectorDown[ k ] == upOldNumerator )  )
                            {
                                vectorDown[ k ] = newnumerator;
                            }               
                        }
                    }//j-j+1
                }//for j < wd - 1
                if ( ( m.getNumber( down, wd - 1 ) == 1 && m.getNumber( up, wd - 2 ) == 1 ) && ( vectorDown[ wd - 1 ] != vectorUp[ wd - 2 ]  ) )//wd-1-wd-2
                {
                    ++ joining;
                    ++ numerator;
                    int newnumerator = numerator;
                    int upOldNumerator = vectorUp[ wd - 2 ];
                    int downOldNumerator = vectorDown[ wd - 1 ];
                    for ( int k = 0; k < wd; ++ k )
                    {
                        if ( ( vectorUp[ k ] == upOldNumerator ) || ( vectorUp[ k ] == downOldNumerator )  )
                        {
                            vectorUp[ k ] = newnumerator;
                        }
                        if ( ( vectorDown[ k ] == downOldNumerator ) || ( vectorDown[ k ] == upOldNumerator )  )
                        {
                            vectorDown[ k ] = newnumerator;
                        }               
                    }
                }//wd-1-wd-2
                if ( ( m.getNumber( down, wd - 1 ) == 1 && m.getNumber( up, wd - 1 ) == 1 ) && ( vectorDown[ wd - 1 ] != vectorUp[ wd - 1 ]  ) )//wd-1-wd-1
                {
                    ++ joining;
                    ++ numerator;
                    int newnumerator = numerator;
                    int upOldNumerator = vectorUp[ wd - 1 ];
                    int downOldNumerator = vectorDown[ wd - 1 ];
                    for ( int k = 0; k < wd; ++ k )
                    {
                        if ( ( vectorUp[ k ] == upOldNumerator ) || ( vectorUp[ k ] == downOldNumerator ) )
                        {
                            vectorUp[ k ] = newnumerator;
                        }
                        if ( ( vectorDown[ k ] == downOldNumerator ) || ( vectorDown[ k ] == upOldNumerator ) )
                        {
                            vectorDown[ k ] = newnumerator;
                        }               
                    }
                }//wd-1-wd-1
            }

            numUpperComponents = numUpperComponents + numDownLineComponents - joining;
            arrayProvider.swap(vectorUp, vectorDown);
            // vectorUp = vectorDown;
        }//i

        arrayProvider.destroy(vectorUp);
        arrayProvider.destroy(vectorDown);

        return numUpperComponents;
    }//getNumberOfAllLevelComponentsBinary
};

#endif

