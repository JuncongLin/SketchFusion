/**
 * @file    MarchingCubes.cpp
 * @author  Thomas Lewiner <thomas.lewiner@polytechnique.org>
 * @author  Math Dept, PUC-Rio
 * @version 0.2
 * @date    12/08/2002
 *
 * @brief   MarchingCubes Algorithm
 */
//________________________________________________


#if !defined(WIN32) || defined(__CYGWIN__)
#pragma implementation
#endif // WIN32
#include "stdafx.h"
#include <math.h>
#include <time.h>
#include <memory.h>
#include <stdlib.h>
#include <float.h>
#include <vector>
#include "MarchingCubes.h"
#include "global.h"
//#include "ply.h"
#include "LookUpTable.h"
#include "IntersectionTest.h"
using namespace std;
// step size of the arrays of vertices and triangles
#define ALLOC_SIZE 65536
#include <fstream>
#include ".\marchingcubes.h"
extern ofstream outfile;
//_____________________________________________________________________________
// print cube for debug
void MarchingCubes::print_cube() { printf( "\t%f %f %f %f %f %f %f %f\n", _cube[0], _cube[1], _cube[2], _cube[3], _cube[4], _cube[5], _cube[6], _cube[7]) ; }
//_____________________________________________________________________________



//_____________________________________________________________________________
// Constructor
MarchingCubes::MarchingCubes( const int size_x /*= -1*/, const int size_y /*= -1*/, const int size_z /*= -1*/ ) :
//-----------------------------------------------------------------------------
  _originalMC(false),
  _ext_data  (false),
  _bDraw (false),
  _size_x    (size_x),
  _size_y    (size_y),
  _size_z    (size_z),
  _data      ((real *)NULL),
  _x_verts   (( int *)NULL),
  _y_verts   (( int *)NULL),
  _z_verts   (( int *)NULL),
  _nverts    (0),
  _ntrigs    (0),
  _Nverts    (0),
  _Ntrigs    (0),
  _vertices  (( MVertex *)NULL),
  _triangles ((Triangle*)NULL)
{}
//_____________________________________________________________________________



//_____________________________________________________________________________
// Destructor
MarchingCubes::~MarchingCubes()
//-----------------------------------------------------------------------------
{
}
//_____________________________________________________________________________



//_____________________________________________________________________________
// main algorithm
void MarchingCubes::run(  )
//-----------------------------------------------------------------------------
{

   for( _k = 0 ; _k < _size_z-1 ; _k++ )
  for( _j = 0 ; _j < _size_y-1 ; _j++ )
  for( _i = 0 ; _i < _size_x-1 ; _i++ )
  {
	if(_cube_type[_i+_j*(_size_x-1)+_k*(_size_x-1)*(_size_y-1)]==0||_cube_type[_i+_j*(_size_x-1)+_k*(_size_x-1)*(_size_y-1)]==-1)
	{
		process_cube( ) ;
	}
  }

}
//_____________________________________________________________________________

void MarchingCubes::set_boundingbox(const CVector3d& vmin,const CVector3d& vmax)
{
	double xlen,ylen,zlen;
	xlen=vmax.x()-vmin.x();
	ylen=vmax.y()-vmin.y();
	zlen=vmax.z()-vmin.z();

	CVector3d vext(xlen,ylen,zlen);

	_bb.Reset(vmin-vext,vmax+vext);
}


//_____________________________________________________________________________
// init temporary structures (must set sizes before call)
void MarchingCubes::init_temps()
//-----------------------------------------------------------------------------
{
  if( !_ext_data )
    _data    = new real [_size_x * _size_y * _size_z] ;


  _x_verts = new int  [_size_x * _size_y * _size_z] ;
  _y_verts = new int  [_size_x * _size_y * _size_z] ;
  _z_verts = new int  [_size_x * _size_y * _size_z] ;

  _cube_type = new int [(_size_x-1) * (_size_y-1) * (_size_z-1)] ;
  _lut_entry = new uchar[(_size_x-1) * (_size_y-1) * (_size_z-1)];


  memset( _x_verts, -1, _size_x * _size_y * _size_z * sizeof( int ) ) ;
  memset( _y_verts, -1, _size_x * _size_y * _size_z * sizeof( int ) ) ;
  memset( _z_verts, -1, _size_x * _size_y * _size_z * sizeof( int ) ) ;
  memset( _cube_type, -1, (_size_x-1) * (_size_y-1) * (_size_z-1) * sizeof( int ) ) ;
  memset( _lut_entry, 0, (_size_x-1) * (_size_y-1) * (_size_z-1) * sizeof( uchar ) ) ;
}
//_____________________________________________________________________________



//_____________________________________________________________________________
// init all structures (must set sizes before call)
void MarchingCubes::init_all ()
//-----------------------------------------------------------------------------
{
  init_temps() ;

  _bDraw=true;

  _nverts = _ntrigs = 0 ;
  _Nverts = _Ntrigs = ALLOC_SIZE ;
  _vertices  = new MVertex  [_Nverts] ;
  _triangles = new Triangle[_Ntrigs] ;
}
//_____________________________________________________________________________



//_____________________________________________________________________________
// clean temporary structures
void MarchingCubes::clean_temps()
//-----------------------------------------------------------------------------
{
  if( !_ext_data )
    delete [] _data;
  delete [] _x_verts;
  delete [] _y_verts;
  delete [] _z_verts;
  delete []_lut_entry;

  if( !_ext_data )
    _data     = (real*)NULL ;
  _x_verts  = (int*)NULL ;
  _y_verts  = (int*)NULL ;
  _z_verts  = (int*)NULL ;
  _lut_entry= (uchar*)NULL ;
}
//_____________________________________________________________________________



//_____________________________________________________________________________
// clean all structures
void MarchingCubes::clean_all()
//-----------------------------------------------------------------------------
{
  delete [] _vertices  ;
  delete [] _triangles ;
  _vertices  = (MVertex   *)NULL ;
  _triangles = (Triangle *)NULL ;
  _nverts = _ntrigs = 0 ;
  _Nverts = _Ntrigs = 0 ;

  _size_x = _size_y = _size_z = -1 ;

  delete []_cube_type;
  _cube_type= (int*)NULL ;

}
//_____________________________________________________________________________



//_____________________________________________________________________________
//_____________________________________________________________________________


//_____________________________________________________________________________
// Compute the intersection points
void MarchingCubes::compute_intersection_points( real iso )
//-----------------------------------------------------------------------------
{
  for( _k = 0 ; _k < _size_z ; _k++ )
  for( _j = 0 ; _j < _size_y ; _j++ )
  for( _i = 0 ; _i < _size_x ; _i++ )
  {
    _cube[0] = get_data( _i, _j, _k ) - iso ;
    if( _i < _size_x - 1 ) _cube[1] = get_data(_i+1, _j , _k ) - iso ;
    else                   _cube[1] = _cube[0] ;

    if( _j < _size_y - 1 ) _cube[3] = get_data( _i ,_j+1, _k ) - iso ;
    else                   _cube[3] = _cube[0] ;

    if( _k < _size_z - 1 ) _cube[4] = get_data( _i , _j ,_k+1) - iso ;
    else                   _cube[4] = _cube[0] ;

    if( fabs( _cube[0] ) < FLT_EPSILON ) _cube[0] = FLT_EPSILON ;
    if( fabs( _cube[1] ) < FLT_EPSILON ) _cube[1] = FLT_EPSILON ;
    if( fabs( _cube[3] ) < FLT_EPSILON ) _cube[3] = FLT_EPSILON ;
    if( fabs( _cube[4] ) < FLT_EPSILON ) _cube[4] = FLT_EPSILON ;

    if( _cube[0] < 0 )
    {
      if( _cube[1] > 0 ) set_x_vert( add_x_vertex( ), _i,_j,_k ) ;
      if( _cube[3] > 0 ) set_y_vert( add_y_vertex( ), _i,_j,_k ) ;
      if( _cube[4] > 0 ) set_z_vert( add_z_vertex( ), _i,_j,_k ) ;
    }
    else
    {
      if( _cube[1] < 0 ) set_x_vert( add_x_vertex( ), _i,_j,_k ) ;
      if( _cube[3] < 0 ) set_y_vert( add_y_vertex( ), _i,_j,_k ) ;
      if( _cube[4] < 0 ) set_z_vert( add_z_vertex( ), _i,_j,_k ) ;
    }
  }
}
//_____________________________________________________________________________





//_____________________________________________________________________________
// Test a face
// if face>0 return true if the face contains a part of the surface
bool MarchingCubes::test_face( schar face )
//-----------------------------------------------------------------------------
{
  real A,B,C,D ;

  switch( face )
  {
  case -1 : case 1 :  A = _cube[0] ;  B = _cube[4] ;  C = _cube[5] ;  D = _cube[1] ;  break ;
  case -2 : case 2 :  A = _cube[1] ;  B = _cube[5] ;  C = _cube[6] ;  D = _cube[2] ;  break ;
  case -3 : case 3 :  A = _cube[2] ;  B = _cube[6] ;  C = _cube[7] ;  D = _cube[3] ;  break ;
  case -4 : case 4 :  A = _cube[3] ;  B = _cube[7] ;  C = _cube[4] ;  D = _cube[0] ;  break ;
  case -5 : case 5 :  A = _cube[0] ;  B = _cube[3] ;  C = _cube[2] ;  D = _cube[1] ;  break ;
  case -6 : case 6 :  A = _cube[4] ;  B = _cube[7] ;  C = _cube[6] ;  D = _cube[5] ;  break ;
  default : printf( "Invalid face code %d\n", face ) ;  print_cube() ;  A = B = C = D = 0 ;
  };

  if( fabs( A*C - B*D ) < FLT_EPSILON )
    return face >= 0 ;
  return face * A * ( A*C - B*D ) >= 0  ;  // face and A invert signs
}
//_____________________________________________________________________________





//_____________________________________________________________________________
// Test the interior of a cube
// if s == 7, return true  if the interior is empty
// if s ==-7, return false if the interior is empty
bool MarchingCubes::test_interior( schar s )
//-----------------------------------------------------------------------------
{
  real t, At=0, Bt=0, Ct=0, Dt=0, a, b ;
  char  test =  0 ;
  char  edge = -1 ; // reference edge of the triangulation

  switch( _case )
  {
  case  4 :
  case 10 :
    a = ( _cube[4] - _cube[0] ) * ( _cube[6] - _cube[2] ) - ( _cube[7] - _cube[3] ) * ( _cube[5] - _cube[1] ) ;
    b =  _cube[2] * ( _cube[4] - _cube[0] ) + _cube[0] * ( _cube[6] - _cube[2] )
             - _cube[1] * ( _cube[7] - _cube[3] ) - _cube[3] * ( _cube[5] - _cube[1] ) ;
    t = - b / (2*a) ;
    if( t<0 || t>1 ) return s>0 ;

    At = _cube[0] + ( _cube[4] - _cube[0] ) * t ;
    Bt = _cube[3] + ( _cube[7] - _cube[3] ) * t ;
    Ct = _cube[2] + ( _cube[6] - _cube[2] ) * t ;
    Dt = _cube[1] + ( _cube[5] - _cube[1] ) * t ;
    break ;

  case  6 :
  case  7 :
  case 12 :
  case 13 :
    switch( _case )
    {
    case  6 : edge = test6 [_config][2] ; break ;
    case  7 : edge = test7 [_config][4] ; break ;
    case 12 : edge = test12[_config][3] ; break ;
    case 13 : edge = tiling13_5_1[_config][_subconfig][0] ; break ;
    }
    switch( edge )
    {
    case  0 :
      t  = _cube[0] / ( _cube[0] - _cube[1] ) ;
      At = 0 ;
      Bt = _cube[3] + ( _cube[2] - _cube[3] ) * t ;
      Ct = _cube[7] + ( _cube[6] - _cube[7] ) * t ;
      Dt = _cube[4] + ( _cube[5] - _cube[4] ) * t ;
      break ;
    case  1 :
      t  = _cube[1] / ( _cube[1] - _cube[2] ) ;
      At = 0 ;
      Bt = _cube[0] + ( _cube[3] - _cube[0] ) * t ;
      Ct = _cube[4] + ( _cube[7] - _cube[4] ) * t ;
      Dt = _cube[5] + ( _cube[6] - _cube[5] ) * t ;
      break ;
    case  2 :
      t  = _cube[2] / ( _cube[2] - _cube[3] ) ;
      At = 0 ;
      Bt = _cube[1] + ( _cube[0] - _cube[1] ) * t ;
      Ct = _cube[5] + ( _cube[4] - _cube[5] ) * t ;
      Dt = _cube[6] + ( _cube[7] - _cube[6] ) * t ;
      break ;
    case  3 :
      t  = _cube[3] / ( _cube[3] - _cube[0] ) ;
      At = 0 ;
      Bt = _cube[2] + ( _cube[1] - _cube[2] ) * t ;
      Ct = _cube[6] + ( _cube[5] - _cube[6] ) * t ;
      Dt = _cube[7] + ( _cube[4] - _cube[7] ) * t ;
      break ;
    case  4 :
      t  = _cube[4] / ( _cube[4] - _cube[5] ) ;
      At = 0 ;
      Bt = _cube[7] + ( _cube[6] - _cube[7] ) * t ;
      Ct = _cube[3] + ( _cube[2] - _cube[3] ) * t ;
      Dt = _cube[0] + ( _cube[1] - _cube[0] ) * t ;
      break ;
    case  5 :
      t  = _cube[5] / ( _cube[5] - _cube[6] ) ;
      At = 0 ;
      Bt = _cube[4] + ( _cube[7] - _cube[4] ) * t ;
      Ct = _cube[0] + ( _cube[3] - _cube[0] ) * t ;
      Dt = _cube[1] + ( _cube[2] - _cube[1] ) * t ;
      break ;
    case  6 :
      t  = _cube[6] / ( _cube[6] - _cube[7] ) ;
      At = 0 ;
      Bt = _cube[5] + ( _cube[4] - _cube[5] ) * t ;
      Ct = _cube[1] + ( _cube[0] - _cube[1] ) * t ;
      Dt = _cube[2] + ( _cube[3] - _cube[2] ) * t ;
      break ;
    case  7 :
      t  = _cube[7] / ( _cube[7] - _cube[4] ) ;
      At = 0 ;
      Bt = _cube[6] + ( _cube[5] - _cube[6] ) * t ;
      Ct = _cube[2] + ( _cube[1] - _cube[2] ) * t ;
      Dt = _cube[3] + ( _cube[0] - _cube[3] ) * t ;
      break ;
    case  8 :
      t  = _cube[0] / ( _cube[0] - _cube[4] ) ;
      At = 0 ;
      Bt = _cube[3] + ( _cube[7] - _cube[3] ) * t ;
      Ct = _cube[2] + ( _cube[6] - _cube[2] ) * t ;
      Dt = _cube[1] + ( _cube[5] - _cube[1] ) * t ;
      break ;
    case  9 :
      t  = _cube[1] / ( _cube[1] - _cube[5] ) ;
      At = 0 ;
      Bt = _cube[0] + ( _cube[4] - _cube[0] ) * t ;
      Ct = _cube[3] + ( _cube[7] - _cube[3] ) * t ;
      Dt = _cube[2] + ( _cube[6] - _cube[2] ) * t ;
      break ;
    case 10 :
      t  = _cube[2] / ( _cube[2] - _cube[6] ) ;
      At = 0 ;
      Bt = _cube[1] + ( _cube[5] - _cube[1] ) * t ;
      Ct = _cube[0] + ( _cube[4] - _cube[0] ) * t ;
      Dt = _cube[3] + ( _cube[7] - _cube[3] ) * t ;
      break ;
    case 11 :
      t  = _cube[3] / ( _cube[3] - _cube[7] ) ;
      At = 0 ;
      Bt = _cube[2] + ( _cube[6] - _cube[2] ) * t ;
      Ct = _cube[1] + ( _cube[5] - _cube[1] ) * t ;
      Dt = _cube[0] + ( _cube[4] - _cube[0] ) * t ;
      break ;
    default : printf( "Invalid edge %d\n", edge ) ;  print_cube() ;  break ;
    }
    break ;

  default : printf( "Invalid ambiguous case %d\n", _case ) ;  print_cube() ;  break ;
  }

  if( At >= 0 ) test ++ ;
  if( Bt >= 0 ) test += 2 ;
  if( Ct >= 0 ) test += 4 ;
  if( Dt >= 0 ) test += 8 ;
  switch( test )
  {
  case  0 : return s>0 ;
  case  1 : return s>0 ;
  case  2 : return s>0 ;
  case  3 : return s>0 ;
  case  4 : return s>0 ;
  case  5 : if( At * Ct - Bt * Dt <  FLT_EPSILON ) return s>0 ; break ;
  case  6 : return s>0 ;
  case  7 : return s<0 ;
  case  8 : return s>0 ;
  case  9 : return s>0 ;
  case 10 : if( At * Ct - Bt * Dt >= FLT_EPSILON ) return s>0 ; break ;
  case 11 : return s<0 ;
  case 12 : return s>0 ;
  case 13 : return s<0 ;
  case 14 : return s<0 ;
  case 15 : return s<0 ;
  }

  return s<0 ;
}
//_____________________________________________________________________________




//_____________________________________________________________________________
// Process a unit cube
void MarchingCubes::process_cube( )
//-----------------------------------------------------------------------------
{
   int ltentry=_lut_entry[_i+_j*(_size_x-1)+_k*(_size_x-1)*(_size_y-1)];
  if( _originalMC )
  {
    char nt = 0 ;
    while( casesClassic[ltentry][3*nt] != -1 ) nt++ ;
    add_triangle( casesClassic[ltentry], nt ) ;
    return ;
  }

  int   v12 = -1 ;
  _case   = cases[ltentry][0] ;
  _config = cases[ltentry][1] ;
  _subconfig = 0 ;

  switch( _case )
  {
  case  0 :
    break ;

  case  1 :
    add_triangle( tiling1[_config], 1 ) ;
    break ;

  case  2 :
    add_triangle( tiling2[_config], 2 ) ;
    break ;

  case  3 :
    if( test_face( test3[_config]) )
      add_triangle( tiling3_2[_config], 4 ) ; // 3.2
    else
      add_triangle( tiling3_1[_config], 2 ) ; // 3.1
    break ;

  case  4 :
    if( test_interior( test4[_config]) )
      add_triangle( tiling4_1[_config], 2 ) ; // 4.1.1
    else
      add_triangle( tiling4_2[_config], 6 ) ; // 4.1.2
    break ;

  case  5 :
    add_triangle( tiling5[_config], 3 ) ;
    break ;

  case  6 :
    if( test_face( test6[_config][0]) )
      add_triangle( tiling6_2[_config], 5 ) ; // 6.2
    else
    {
      if( test_interior( test6[_config][1]) )
        add_triangle( tiling6_1_1[_config], 3 ) ; // 6.1.1
      else
        add_triangle( tiling6_1_2[_config], 7 ) ; // 6.1.2
    }
    break ;

  case  7 :
    if( test_face( test7[_config][0] ) ) _subconfig +=  1 ;
    if( test_face( test7[_config][1] ) ) _subconfig +=  2 ;
    if( test_face( test7[_config][2] ) ) _subconfig +=  4 ;
    switch( _subconfig )
      {
      case 0 :
        add_triangle( tiling7_1[_config], 3 ) ; break ;
      case 1 :
        add_triangle( tiling7_2[_config][0], 5 ) ; break ;
      case 2 :
        add_triangle( tiling7_2[_config][1], 5 ) ; break ;
      case 3 :
        v12 = add_c_vertex() ;
        add_triangle( tiling7_3[_config][0], 9, v12 ) ; break ;
      case 4 :
        add_triangle( tiling7_2[_config][2], 5 ) ; break ;
      case 5 :
        v12 = add_c_vertex() ;
        add_triangle( tiling7_3[_config][1], 9, v12 ) ; break ;
      case 6 :
        v12 = add_c_vertex() ;
        add_triangle( tiling7_3[_config][2], 9, v12 ) ; break ;
      case 7 :
        if( test_interior( test7[_config][3]) )
          add_triangle( tiling7_4_2[_config], 9 ) ;
        else
          add_triangle( tiling7_4_1[_config], 5 ) ;
        break ;
      };
    break ;

  case  8 :
    add_triangle( tiling8[_config], 2 ) ;
    break ;

  case  9 :
    add_triangle( tiling9[_config], 4 ) ;
    break ;

  case 10 :
    if( test_face( test10[_config][0]) )
    {
      if( test_face( test10[_config][1]) )
        add_triangle( tiling10_1_1_[_config], 4 ) ; // 10.1.1
      else
      {
        v12 = add_c_vertex() ;
        add_triangle( tiling10_2[_config], 8, v12 ) ; // 10.2
      }
    }
    else
    {
      if( test_face( test10[_config][1]) )
      {
        v12 = add_c_vertex() ;
        add_triangle( tiling10_2_[_config], 8, v12 ) ; // 10.2
      }
      else
      {
        if( test_interior( test10[_config][2]) )
          add_triangle( tiling10_1_1[_config], 4 ) ; // 10.1.1
        else
          add_triangle( tiling10_1_2[_config], 8 ) ; // 10.1.2
      }
    }
    break ;

  case 11 :
    add_triangle( tiling11[_config], 4 ) ;
    break ;

  case 12 :
    if( test_face( test12[_config][0]) )
    {
      if( test_face( test12[_config][1]) )
        add_triangle( tiling12_1_1_[_config], 4 ) ; // 12.1.1
      else
      {
        v12 = add_c_vertex() ;
        add_triangle( tiling12_2[_config], 8, v12 ) ; // 12.2
      }
    }
    else
    {
      if( test_face( test12[_config][1]) )
      {
        v12 = add_c_vertex() ;
        add_triangle( tiling12_2_[_config], 8, v12 ) ; // 12.2
      }
      else
      {
        if( test_interior( test12[_config][2]) )
          add_triangle( tiling12_1_1[_config], 4 ) ; // 12.1.1
        else
          add_triangle( tiling12_1_2[_config], 8 ) ; // 12.1.2
      }
    }
    break ;

  case 13 :
    if( test_face( test13[_config][0] ) ) _subconfig +=  1 ;
    if( test_face( test13[_config][1] ) ) _subconfig +=  2 ;
    if( test_face( test13[_config][2] ) ) _subconfig +=  4 ;
    if( test_face( test13[_config][3] ) ) _subconfig +=  8 ;
    if( test_face( test13[_config][4] ) ) _subconfig += 16 ;
    if( test_face( test13[_config][5] ) ) _subconfig += 32 ;
    switch( subconfig13[_subconfig] )
    {
      case 0 :/* 13.1 */
        add_triangle( tiling13_1[_config], 4 ) ; break ;

      case 1 :/* 13.2 */
        add_triangle( tiling13_2[_config][0], 6 ) ; break ;
      case 2 :/* 13.2 */
        add_triangle( tiling13_2[_config][1], 6 ) ; break ;
      case 3 :/* 13.2 */
        add_triangle( tiling13_2[_config][2], 6 ) ; break ;
      case 4 :/* 13.2 */
        add_triangle( tiling13_2[_config][3], 6 ) ; break ;
      case 5 :/* 13.2 */
        add_triangle( tiling13_2[_config][4], 6 ) ; break ;
      case 6 :/* 13.2 */
        add_triangle( tiling13_2[_config][5], 6 ) ; break ;

      case 7 :/* 13.3 */
        v12 = add_c_vertex() ;
        add_triangle( tiling13_3[_config][0], 10, v12 ) ; break ;
      case 8 :/* 13.3 */
        v12 = add_c_vertex() ;
        add_triangle( tiling13_3[_config][1], 10, v12 ) ; break ;
      case 9 :/* 13.3 */
        v12 = add_c_vertex() ;
        add_triangle( tiling13_3[_config][2], 10, v12 ) ; break ;
      case 10 :/* 13.3 */
        v12 = add_c_vertex() ;
        add_triangle( tiling13_3[_config][3], 10, v12 ) ; break ;
      case 11 :/* 13.3 */
        v12 = add_c_vertex() ;
        add_triangle( tiling13_3[_config][4], 10, v12 ) ; break ;
      case 12 :/* 13.3 */
        v12 = add_c_vertex() ;
        add_triangle( tiling13_3[_config][5], 10, v12 ) ; break ;
      case 13 :/* 13.3 */
        v12 = add_c_vertex() ;
        add_triangle( tiling13_3[_config][6], 10, v12 ) ; break ;
      case 14 :/* 13.3 */
        v12 = add_c_vertex() ;
        add_triangle( tiling13_3[_config][7], 10, v12 ) ; break ;
      case 15 :/* 13.3 */
        v12 = add_c_vertex() ;
        add_triangle( tiling13_3[_config][8], 10, v12 ) ; break ;
      case 16 :/* 13.3 */
        v12 = add_c_vertex() ;
        add_triangle( tiling13_3[_config][9], 10, v12 ) ; break ;
      case 17 :/* 13.3 */
        v12 = add_c_vertex() ;
        add_triangle( tiling13_3[_config][10], 10, v12 ) ; break ;
      case 18 :/* 13.3 */
        v12 = add_c_vertex() ;
        add_triangle( tiling13_3[_config][11], 10, v12 ) ; break ;

      case 19 :/* 13.4 */
        v12 = add_c_vertex() ;
        add_triangle( tiling13_4[_config][0], 12, v12 ) ; break ;
      case 20 :/* 13.4 */
        v12 = add_c_vertex() ;
        add_triangle( tiling13_4[_config][1], 12, v12 ) ; break ;
      case 21 :/* 13.4 */
        v12 = add_c_vertex() ;
        add_triangle( tiling13_4[_config][2], 12, v12 ) ; break ;
      case 22 :/* 13.4 */
        v12 = add_c_vertex() ;
        add_triangle( tiling13_4[_config][3], 12, v12 ) ; break ;

      case 23 :/* 13.5 */
        _subconfig = 0 ;
        if( test_interior( test13[_config][6] ) )
          add_triangle( tiling13_5_1[_config][0], 6 ) ;
        else
          add_triangle( tiling13_5_2[_config][0], 10 ) ;
        break ;
      case 24 :/* 13.5 */
        _subconfig = 1 ;
        if( test_interior( test13[_config][6] ) )
          add_triangle( tiling13_5_1[_config][1], 6 ) ;
        else
          add_triangle( tiling13_5_2[_config][1], 10 ) ;
        break ;
      case 25 :/* 13.5 */
        _subconfig = 2 ;
        if( test_interior( test13[_config][6] ) )
          add_triangle( tiling13_5_1[_config][2], 6 ) ;
        else
          add_triangle( tiling13_5_2[_config][2], 10 ) ;
        break ;
      case 26 :/* 13.5 */
        _subconfig = 3 ;
        if( test_interior( test13[_config][6] ) )
          add_triangle( tiling13_5_1[_config][3], 6 ) ;
        else
          add_triangle( tiling13_5_2[_config][3], 10 ) ;
        break ;

      case 27 :/* 13.3 */
        v12 = add_c_vertex() ;
        add_triangle( tiling13_3_[_config][0], 10, v12 ) ; break ;
      case 28 :/* 13.3 */
        v12 = add_c_vertex() ;
        add_triangle( tiling13_3_[_config][1], 10, v12 ) ; break ;
      case 29 :/* 13.3 */
        v12 = add_c_vertex() ;
        add_triangle( tiling13_3_[_config][2], 10, v12 ) ; break ;
      case 30 :/* 13.3 */
        v12 = add_c_vertex() ;
        add_triangle( tiling13_3_[_config][3], 10, v12 ) ; break ;
      case 31 :/* 13.3 */
        v12 = add_c_vertex() ;
        add_triangle( tiling13_3_[_config][4], 10, v12 ) ; break ;
      case 32 :/* 13.3 */
        v12 = add_c_vertex() ;
        add_triangle( tiling13_3_[_config][5], 10, v12 ) ; break ;
      case 33 :/* 13.3 */
        v12 = add_c_vertex() ;
        add_triangle( tiling13_3_[_config][6], 10, v12 ) ; break ;
      case 34 :/* 13.3 */
        v12 = add_c_vertex() ;
        add_triangle( tiling13_3_[_config][7], 10, v12 ) ; break ;
      case 35 :/* 13.3 */
        v12 = add_c_vertex() ;
        add_triangle( tiling13_3_[_config][8], 10, v12 ) ; break ;
      case 36 :/* 13.3 */
        v12 = add_c_vertex() ;
        add_triangle( tiling13_3_[_config][9], 10, v12 ) ; break ;
      case 37 :/* 13.3 */
        v12 = add_c_vertex() ;
        add_triangle( tiling13_3_[_config][10], 10, v12 ) ; break ;
      case 38 :/* 13.3 */
        v12 = add_c_vertex() ;
        add_triangle( tiling13_3_[_config][11], 10, v12 ) ; break ;

      case 39 :/* 13.2 */
        add_triangle( tiling13_2_[_config][0], 6 ) ; break ;
      case 40 :/* 13.2 */
        add_triangle( tiling13_2_[_config][1], 6 ) ; break ;
      case 41 :/* 13.2 */
        add_triangle( tiling13_2_[_config][2], 6 ) ; break ;
      case 42 :/* 13.2 */
        add_triangle( tiling13_2_[_config][3], 6 ) ; break ;
      case 43 :/* 13.2 */
        add_triangle( tiling13_2_[_config][4], 6 ) ; break ;
      case 44 :/* 13.2 */
        add_triangle( tiling13_2_[_config][5], 6 ) ; break ;

      case 45 :/* 13.1 */
        add_triangle( tiling13_1_[_config], 4 ) ; break ;

      default :
        printf("Marching Cubes: Impossible case 13?\n" ) ;  print_cube() ;
      }
      break ;

  case 14 :
    add_triangle( tiling14[_config], 4 ) ;
    break ;
  };
}
//_____________________________________________________________________________



//_____________________________________________________________________________
// Adding triangles
void MarchingCubes::add_triangle( const char* trig, char n, int v12 )
//-----------------------------------------------------------------------------
{
  int    tv[3] ;

  for( int t = 0 ; t < 3*n ; t++ )
  {
    switch( trig[t] )
    {
    case  0 : tv[ t % 3 ] = get_x_vert( _i , _j , _k ) ; break ;
    case  1 : tv[ t % 3 ] = get_y_vert(_i+1, _j , _k ) ; break ;
    case  2 : tv[ t % 3 ] = get_x_vert( _i ,_j+1, _k ) ; break ;
    case  3 : tv[ t % 3 ] = get_y_vert( _i , _j , _k ) ; break ;
    case  4 : tv[ t % 3 ] = get_x_vert( _i , _j ,_k+1) ; break ;
    case  5 : tv[ t % 3 ] = get_y_vert(_i+1, _j ,_k+1) ; break ;
    case  6 : tv[ t % 3 ] = get_x_vert( _i ,_j+1,_k+1) ; break ;
    case  7 : tv[ t % 3 ] = get_y_vert( _i , _j ,_k+1) ; break ;
    case  8 : tv[ t % 3 ] = get_z_vert( _i , _j , _k ) ; break ;
    case  9 : tv[ t % 3 ] = get_z_vert(_i+1, _j , _k ) ; break ;
    case 10 : tv[ t % 3 ] = get_z_vert(_i+1,_j+1, _k ) ; break ;
    case 11 : tv[ t % 3 ] = get_z_vert( _i ,_j+1, _k ) ; break ;
    case 12 : tv[ t % 3 ] = v12 ; break ;
    default : break ;
    }

    if( tv[t%3] == -1 )
    {
      printf("Marching Cubes: invalid triangle %d\n", _ntrigs+1) ;
      print_cube() ;
    }

    if( t%3 == 2 )
    {
      if( _ntrigs >= _Ntrigs )
      {
        Triangle *temp = _triangles ;
        _triangles = new Triangle[ 2*_Ntrigs ] ;
        memcpy( _triangles, temp, _Ntrigs*sizeof(Triangle) ) ;
        delete[] temp ;
        printf("%d allocated triangles\n", _Ntrigs) ;
        _Ntrigs *= 2 ;
      }

      Triangle *T = _triangles + _ntrigs++ ;
      T->v1    = tv[0] ;
      T->v2    = tv[1] ;
      T->v3    = tv[2] ;
    }
  }
}
//_____________________________________________________________________________


CVector3d MarchingCubes::get_grid_vert( const int i, const int j, const int k )
{
    CVector3d gridPos;

	CVector3d startPos=_bb.GetMinCorner();

	gridPos=startPos+CVector3d(((double)i/(double)_size_x)*_bb.XLength(),((double)j/(double)_size_y)*_bb.YLength(),((double)k/(double)_size_z)*_bb.ZLength());

	return gridPos;
}

//_____________________________________________________________________________
// Calculating gradient

real MarchingCubes::get_x_grad( const int i, const int j, const int k ) const
//-----------------------------------------------------------------------------
{
  if( i > 0 )
  {
    if ( i < _size_x - 1 )
      return ( get_data( i+1, j, k ) - get_data( i-1, j, k ) ) / 2 ;
    else
      return get_data( i, j, k ) - get_data( i-1, j, k ) ;
  }
  else
    return get_data( i+1, j, k ) - get_data( i, j, k ) ;
}
//-----------------------------------------------------------------------------

real MarchingCubes::get_y_grad( const int i, const int j, const int k ) const
//-----------------------------------------------------------------------------
{
  if( j > 0 )
  {
    if ( j < _size_y - 1 )
      return ( get_data( i, j+1, k ) - get_data( i, j-1, k ) ) / 2 ;
    else
      return get_data( i, j, k ) - get_data( i, j-1, k ) ;
  }
  else
    return get_data( i, j+1, k ) - get_data( i, j, k ) ;
}
//-----------------------------------------------------------------------------

real MarchingCubes::get_z_grad( const int i, const int j, const int k ) const
//-----------------------------------------------------------------------------
{
  if( k > 0 )
  {
    if ( k < _size_z - 1 )
      return ( get_data( i, j, k+1 ) - get_data( i, j, k-1 ) ) / 2 ;
    else
      return get_data( i, j, k ) - get_data( i, j, k-1 ) ;
  }
  else
    return get_data( i, j, k+1 ) - get_data( i, j, k ) ;
}
//_____________________________________________________________________________


//_____________________________________________________________________________
// Adding vertices

void MarchingCubes::test_vertex_addition()
{
  if( _nverts >= _Nverts )
  {
    MVertex *temp = _vertices ;
    _vertices = new MVertex[ _Nverts*2 ] ;
    memcpy( _vertices, temp, _Nverts*sizeof(MVertex) ) ;
    delete[] temp ;
    printf("%d allocated vertices\n", _Nverts) ;
    _Nverts *= 2 ;
  }
}


int MarchingCubes::add_x_vertex( )
//-----------------------------------------------------------------------------
{
  test_vertex_addition() ;
  MVertex *vert = _vertices + _nverts++ ;

  real u = ( _cube[0] ) / ( _cube[0] - _cube[1] ) ;

  vert->x      = (real)(_i+u)*_bb.XLength()/_size_x+_bb.XMin();
  vert->y      = (real) _j*_bb.YLength()/_size_y+_bb.YMin() ;
  vert->z      = (real) _k*_bb.ZLength()/_size_z+_bb.ZMin();

  vert->nx = (1-u)*get_x_grad(_i,_j,_k) + u*get_x_grad(_i+1,_j,_k) ;
  vert->ny = (1-u)*get_y_grad(_i,_j,_k) + u*get_y_grad(_i+1,_j,_k) ;
  vert->nz = (1-u)*get_z_grad(_i,_j,_k) + u*get_z_grad(_i+1,_j,_k) ;

  u = (real) sqrt( vert->nx * vert->nx + vert->ny * vert->ny +vert->nz * vert->nz ) ;
  if( u > 0 )
  {
    vert->nx /= u ;
    vert->ny /= u ;
    vert->nz /= u ;
  }


  return _nverts-1 ;
}
//-----------------------------------------------------------------------------

int MarchingCubes::add_y_vertex( )
//-----------------------------------------------------------------------------
{
  test_vertex_addition() ;
  MVertex *vert = _vertices + _nverts++ ;

  real u = ( _cube[0] ) / ( _cube[0] - _cube[3] ) ;

  vert->x      = (real) _i*_bb.XLength()/_size_x+_bb.XMin() ;
  vert->y      = (real)(_j+u)*_bb.YLength()/_size_y+_bb.YMin();
  vert->z      = (real) _k *_bb.ZLength()/_size_z+_bb.ZMin();

  vert->nx = (1-u)*get_x_grad(_i,_j,_k) + u*get_x_grad(_i,_j+1,_k) ;
  vert->ny = (1-u)*get_y_grad(_i,_j,_k) + u*get_y_grad(_i,_j+1,_k) ;
  vert->nz = (1-u)*get_z_grad(_i,_j,_k) + u*get_z_grad(_i,_j+1,_k) ;

  u = (real) sqrt( vert->nx * vert->nx + vert->ny * vert->ny +vert->nz * vert->nz ) ;
  if( u > 0 )
  {
    vert->nx /= u ;
    vert->ny /= u ;
    vert->nz /= u ;
  }

  return _nverts-1 ;
}
//-----------------------------------------------------------------------------

int MarchingCubes::add_z_vertex( )
//-----------------------------------------------------------------------------
{
  test_vertex_addition() ;
  MVertex *vert = _vertices + _nverts++ ;

  real u = ( _cube[0] ) / ( _cube[0] - _cube[4] ) ;

  vert->x      = (real) _i*_bb.XLength()/_size_x+_bb.XMin() ;
  vert->y      = (real) _j *_bb.YLength()/_size_y+_bb.YMin();
  vert->z      = (real)(_k+u)*_bb.ZLength()/_size_z+_bb.ZMin();

  vert->nx = (1-u)*get_x_grad(_i,_j,_k) + u*get_x_grad(_i,_j,_k+1) ;
  vert->ny = (1-u)*get_y_grad(_i,_j,_k) + u*get_y_grad(_i,_j,_k+1) ;
  vert->nz = (1-u)*get_z_grad(_i,_j,_k) + u*get_z_grad(_i,_j,_k+1) ;

  u = (real) sqrt( vert->nx * vert->nx + vert->ny * vert->ny +vert->nz * vert->nz ) ;
  if( u > 0 )
  {
    vert->nx /= u ;
    vert->ny /= u ;
    vert->nz /= u ;
  }

  return _nverts-1 ;
}


int MarchingCubes::add_c_vertex( )
//-----------------------------------------------------------------------------
{
  test_vertex_addition() ;
  MVertex *vert = _vertices + _nverts++ ;

  real u = 0 ;
  int   vid ;

  vert->x = vert->y = vert->z =  vert->nx = vert->ny = vert->nz = 0 ;

  // Computes the average of the intersection points of the cube
  vid = get_x_vert( _i , _j , _k ) ;
  if( vid != -1 ) { ++u ; const MVertex &v = _vertices[vid] ; vert->x += v.x ;  vert->y += v.y ;  vert->z += v.z ;  vert->nx += v.nx ; vert->ny += v.ny ; vert->nz += v.nz ; }
  vid = get_y_vert(_i+1, _j , _k ) ;
  if( vid != -1 ) { ++u ; const MVertex &v = _vertices[vid] ; vert->x += v.x ;  vert->y += v.y ;  vert->z += v.z ;  vert->nx += v.nx ; vert->ny += v.ny ; vert->nz += v.nz ; }
  vid = get_x_vert( _i ,_j+1, _k ) ;
  if( vid != -1 ) { ++u ; const MVertex &v = _vertices[vid] ; vert->x += v.x ;  vert->y += v.y ;  vert->z += v.z ;  vert->nx += v.nx ; vert->ny += v.ny ; vert->nz += v.nz ; }
  vid = get_y_vert( _i , _j , _k ) ;
  if( vid != -1 ) { ++u ; const MVertex &v = _vertices[vid] ; vert->x += v.x ;  vert->y += v.y ;  vert->z += v.z ;  vert->nx += v.nx ; vert->ny += v.ny ; vert->nz += v.nz ; }
  vid = get_x_vert( _i , _j ,_k+1) ;
  if( vid != -1 ) { ++u ; const MVertex &v = _vertices[vid] ; vert->x += v.x ;  vert->y += v.y ;  vert->z += v.z ;  vert->nx += v.nx ; vert->ny += v.ny ; vert->nz += v.nz ; }
  vid = get_y_vert(_i+1, _j ,_k+1) ;
  if( vid != -1 ) { ++u ; const MVertex &v = _vertices[vid] ; vert->x += v.x ;  vert->y += v.y ;  vert->z += v.z ;  vert->nx += v.nx ; vert->ny += v.ny ; vert->nz += v.nz ; }
  vid = get_x_vert( _i ,_j+1,_k+1) ;
  if( vid != -1 ) { ++u ; const MVertex &v = _vertices[vid] ; vert->x += v.x ;  vert->y += v.y ;  vert->z += v.z ;  vert->nx += v.nx ; vert->ny += v.ny ; vert->nz += v.nz ; }
  vid = get_y_vert( _i , _j ,_k+1) ;
  if( vid != -1 ) { ++u ; const MVertex &v = _vertices[vid] ; vert->x += v.x ;  vert->y += v.y ;  vert->z += v.z ;  vert->nx += v.nx ; vert->ny += v.ny ; vert->nz += v.nz ; }
  vid = get_z_vert( _i , _j , _k ) ;
  if( vid != -1 ) { ++u ; const MVertex &v = _vertices[vid] ; vert->x += v.x ;  vert->y += v.y ;  vert->z += v.z ;  vert->nx += v.nx ; vert->ny += v.ny ; vert->nz += v.nz ; }
  vid = get_z_vert(_i+1, _j , _k ) ;
  if( vid != -1 ) { ++u ; const MVertex &v = _vertices[vid] ; vert->x += v.x ;  vert->y += v.y ;  vert->z += v.z ;  vert->nx += v.nx ; vert->ny += v.ny ; vert->nz += v.nz ; }
  vid = get_z_vert(_i+1,_j+1, _k ) ;
  if( vid != -1 ) { ++u ; const MVertex &v = _vertices[vid] ; vert->x += v.x ;  vert->y += v.y ;  vert->z += v.z ;  vert->nx += v.nx ; vert->ny += v.ny ; vert->nz += v.nz ; }
  vid = get_z_vert( _i ,_j+1, _k ) ;
  if( vid != -1 ) { ++u ; const MVertex &v = _vertices[vid] ; vert->x += v.x ;  vert->y += v.y ;  vert->z += v.z ;  vert->nx += v.nx ; vert->ny += v.ny ; vert->nz += v.nz ; }

  vert->x  /= u ;
  vert->y  /= u ;
  vert->z  /= u ;

  u = (real) sqrt( vert->nx * vert->nx + vert->ny * vert->ny +vert->nz * vert->nz ) ;
  if( u > 0 )
  {
    vert->nx /= u ;
    vert->ny /= u ;
    vert->nz /= u ;
  }

  return _nverts-1 ;
}
//_____________________________________________________________________________



//_____________________________________________________________________________
//_____________________________________________________________________________




//_____________________________________________________________________________
// Grid exportation
void MarchingCubes::writeISO(const char *fn )
//-----------------------------------------------------------------------------
{
  unsigned char buf[sizeof(float)] ;

  FILE *fp = fopen( fn, "wb" ) ;

  // header
  * (int*) buf = _size_x ;
  fwrite(buf, sizeof(float), 1, fp);
  * (int*) buf = _size_y ;
  fwrite(buf, sizeof(float), 1, fp);
  * (int*) buf = _size_z ;
  fwrite(buf, sizeof(float), 1, fp);

  * (float*) buf = -1.0f ;
  fwrite(buf, sizeof(float), 1, fp);
  * (float*) buf =  1.0f ;
  fwrite(buf, sizeof(float), 1, fp);
  * (float*) buf = -1.0f ;
  fwrite(buf, sizeof(float), 1, fp);
  * (float*) buf =  1.0f ;
  fwrite(buf, sizeof(float), 1, fp);
  * (float*) buf = -1.0f ;
  fwrite(buf, sizeof(float), 1, fp);
  * (float*) buf =  1.0f ;
  fwrite(buf, sizeof(float), 1, fp);

  for( int i = 0 ; i < _size_x ; i++ )
  {
    for( int j = 0 ; j < _size_y ; j++ )
    {
      for( int k = 0 ; k < _size_z ; k++ )
      {
        * (float*) buf = (float)get_data( i,j,k ) ;
        fwrite(buf, sizeof(float), 1, fp);
      }
    }
  }

  fclose(fp) ;
}
//_____________________________________________________________________________


//_____________________________________________________________________________
// Open Inventor / VRML 1.0 ascii exportation
void MarchingCubes::writeIV(const char *fn )
//-----------------------------------------------------------------------------
{
  FILE *fp = fopen( fn, "w" ) ;
  int   i ;

  printf("Marching Cubes::exportIV(%s)...", fn) ;

  fprintf( fp, "#Inventor V2.1 ascii \n\nSeparator { \n    ShapeHints {\n        vertexOrdering  COUNTERCLOCKWISE\n        shapeType       UNKNOWN_SHAPE_TYPE\n        creaseAngle     0.0\n    }\n Coordinate3 { \n point [  \n" ) ;
  for ( i = 0; i < _nverts; i++ )
    fprintf( fp, " %f %f %f,\n", _vertices[i].x, _vertices[i].y, _vertices[i].z ) ;
  printf("   %d vertices written\n", _nverts ) ;

  fprintf( fp, "\n ] \n} \nNormal { \nvector [ \n" ) ;
  for ( i = 0; i < _nverts; i++ )
    fprintf( fp, " %f %f %f,\n", _vertices[i].nx, _vertices[i].ny, _vertices[i].nz ) ;

  fprintf( fp, "\n ] \n} \nIndexedFaceSet { \ncoordIndex [ \n" ) ;
  for ( i = 0; i < _ntrigs; i++ )
    fprintf( fp, "%d, %d, %d, -1,\n", _triangles[i].v1, _triangles[i].v2, _triangles[i].v3 ) ;

  fprintf( fp, " ] \n } \n } \n" ) ;
  fclose( fp ) ;
  printf("   %d triangles written\n", _ntrigs ) ;
}
//_____________________________________________________________________________

void MarchingCubes::rend()
{
	if(!_bDraw)return ;
	int i,j,k;
	uchar *ltentry;
	glEnable(GL_COLOR_MATERIAL);
	//glPointSize(2.0);
	//glEnable(GL_POINT_SMOOTH);
	CVector3d step(_bb.XLength()/_size_x,_bb.YLength()/_size_y,_bb.ZLength()/_size_z);
	CVector3d halfstep=step*0.5;
	CVector3d StartPos=_bb.GetMinCorner()+halfstep;
	CVector3d gridPos;
	for( k = 0 ; k < _size_z-1 ; k++ )
		for( j = 0 ; j < _size_y-1 ; j++ )
			for( i = 0 ; i < _size_x-1 ; i++ )
			{
				if(_cube_type[i+j*(_size_x-1)+k*(_size_x-1)*(_size_y-1)]==3)continue;
				//if(_cube_type[i+j*(_size_x-1)+k*(_size_x-1)*(_size_y-1)]==3)continue;
				glPolygonMode(GL_FRONT,GL_FILL);
				switch(_cube_type[i+j*(_size_x-1)+k*(_size_x-1)*(_size_y-1)])
				{
				case 0:
				case -1:
					glColor3f(0.0,1.0,0.0);break;
				case 1:
					//outfile<<"boundary cubes"<<endl;
					glColor3f(1.0,0.0,0.0);break;
				case 2:
					glColor3f(0.0,0.0,1.0);break;
				default:
					glPolygonMode(GL_FRONT,GL_LINE);
					glColor3f(0.0,0.0,0.0);break;
				}
				gridPos=StartPos+CVector3d(i*step[0],j*step[1],k*step[2]);
				//glVertex3dv(gridPos.GetArray());
				CBox3 box(gridPos[0],gridPos[1],gridPos[2],gridPos[0]+step[0],gridPos[1]+step[1],gridPos[2]+step[2]);
				box.Display();

                
			}
	//glPolygonMode(GL_FRONT,GL_LINE);
	//_bb.Display();
	glDisable(GL_COLOR_MATERIAL);


}
void MarchingCubes::determine_cubes_cases(real iso)
{
	int i,j,k,p;
	uchar *ltentry;

	//outfile<<"determine cube cases"<<endl;
	//outfile<<"Dimension <"<<_size_x<<","<<_size_y<<","<<_size_z<<">"<<endl;

	compute_intersection_points( iso ) ;


	for( k = 0 ; k < _size_z-1 ; k++ )
		for( j = 0 ; j < _size_y-1 ; j++ )
			for( i = 0 ; i < _size_x-1 ; i++ )
			{
				//outfile<<"dt i="<<i<<","<<"j="<<j<<","<<"k="<<k<<endl;
				ltentry=&_lut_entry[i+j*(_size_x-1)+k*(_size_x-1)*(_size_y-1)];
				for(  p = 0 ; p < 8 ; ++p )
				{
					//outfile<<"p="<<p<<";";
					_cube[p] = get_data( i+((p^(p>>1))&1), j+((p>>1)&1), k+((p>>2)&1) ) - iso ;
					if( fabs( _cube[p] ) < FLT_EPSILON ) _cube[p] = FLT_EPSILON ;
					if( _cube[p] > 0 ) *ltentry += 1 << p ;
				}
				if (*ltentry==255||*ltentry==0)
					_cube_type[i+j*(_size_x-1)+k*(_size_x-1)*(_size_y-1)]=3;
			}

}

void MarchingCubes::clarify_cubes(const vector<CPoint3>& bound,const vector<CTriangle3>& triband)
{
	int m,n,p,i,j,k,vg1[3],vg2[3],lowx,lowy,lowz,highx,highy,highz,tmp;
	CPoint3 p1,p2; 
	CTriangle3 tri;
	CVector3d vmin,vmax,step,StartPos;

	//outfile<<"clarify cube"<<endl;
	//outfile<<"Dimension <"<<_size_x<<","<<_size_y<<","<<_size_z<<">"<<endl;


	Cube *invCubes_Cur,*invCubes_Bak,*invCubes_Tmp,*BoundCubes;
	int _nInvCubes_Cur=0,_nInvCubes_Bak,_nBoundCubes=0;

	BoundCubes=new Cube[3*_size_x*_size_y];
	invCubes_Cur=new Cube[_size_x*_size_y];
	invCubes_Bak=new Cube[_size_x*_size_y];

	StartPos=_bb.GetMinCorner();

	step.Set(_bb.XLength()/(_size_x-1),_bb.YLength()/(_size_y-1),_bb.ZLength()/(_size_z-1));

	for (m=0;m<bound.size()-1;m++)
	{
		p1=bound.at(m);
		p2=bound.at(m+1);

		vg1[0]=(int)((p1[0]-StartPos[0])/step[0]);
		vg1[1]=(int)((p1[1]-StartPos[1])/step[1]);
		vg1[2]=(int)((p1[2]-StartPos[2])/step[2]);

		vg2[0]=(int)((p2[0]-StartPos[0])/step[0]);
		vg2[1]=(int)((p2[1]-StartPos[1])/step[1]);
		vg2[2]=(int)((p2[2]-StartPos[2])/step[2]);

		if(vg1[0]>vg2[0]){tmp=vg2[0];vg2[0]=vg1[0];vg1[0]=tmp;}
		if(vg1[1]>vg2[1]){tmp=vg2[1];vg2[1]=vg1[1];vg1[1]=tmp;}
		if(vg1[2]>vg2[2]){tmp=vg2[2];vg2[2]=vg1[2];vg1[2]=tmp;}

		for (i=vg1[0];i<=vg2[0];i++)
		{
			for (j=vg1[1];j<=vg2[1];j++)
			{
				for (k=vg1[2];k<=vg2[2];k++)
				{
					vmin.Set(i*step[0],j*step[1],k*step[2]);
					vmin+=StartPos;
					vmax=vmin+step;
					CBox3 box;
					box.Reset(vmin,vmax);
					if(CIntersectionTest::Box_LineSegment(box,p1,p2,StartPos,_bb.MaxAxisLength()))
					{
						_cube_type[i+j*(_size_x-1)+k*(_size_x-1)*(_size_y-1)]=1;

						BoundCubes[_nBoundCubes].i=i;
						BoundCubes[_nBoundCubes].j=j;
						BoundCubes[_nBoundCubes++].k=k;
					}
				}
			}
		}
	}
	p1=bound.at(m);
	p2=bound.at(0);

	vg1[0]=(int)((p1[0]-StartPos[0])/step[0]);
	vg1[1]=(int)((p1[1]-StartPos[1])/step[1]);
	vg1[2]=(int)((p1[2]-StartPos[2])/step[2]);

	vg2[0]=(int)((p2[0]-StartPos[0])/step[0]);
	vg2[1]=(int)((p2[1]-StartPos[1])/step[1]);
	vg2[2]=(int)((p2[2]-StartPos[2])/step[2]);

	if(vg1[0]>vg2[0]){tmp=vg2[0];vg2[0]=vg1[0];vg1[0]=tmp;}
	if(vg1[1]>vg2[1]){tmp=vg2[1];vg2[1]=vg1[1];vg1[1]=tmp;}
	if(vg1[2]>vg2[2]){tmp=vg2[2];vg2[2]=vg1[2];vg1[2]=tmp;}


	for (i=vg1[0];i<=vg2[0];i++)
	{
		for (j=vg1[1];j<=vg2[1];j++)
		{
			for (k=vg1[2];k<=vg2[2];k++)
			{
				vmin.Set(i*step[0],j*step[1],k*step[2]);
				vmin+=StartPos;
				vmax=vmin+step;
				CBox3 box;
				box.Reset(vmin,vmax);
				if(CIntersectionTest::Box_LineSegment(box,p1,p2,StartPos,_bb.MaxAxisLength()))
				{
					_cube_type[i+j*(_size_x-1)+k*(_size_x-1)*(_size_y-1)]=1;

					BoundCubes[_nBoundCubes].i=i;
					BoundCubes[_nBoundCubes].j=j;
					BoundCubes[_nBoundCubes++].k=k;
				}
			}
		}
	}
	int _n=_nBoundCubes;
	for (m=0;m<_n;m++)
	{
		lowx = MAX(0,BoundCubes[m].i-1);
		highx = MIN(_size_x-1,BoundCubes[m].i+1);
		lowy = MAX(0,BoundCubes[m].j-1);
		highy = MIN(_size_y-1,BoundCubes[m].j+1);
		lowz = MAX(0,BoundCubes[m].k-1);
		highz = MIN(_size_z-1,BoundCubes[m].k+1);

		for(i=lowx;i<=highx;i++)
		{
			for (j=lowy;j<=highy;j++)
			{
				for (k=lowz;k<=highz;k++)
				{
					if(_cube_type[i+j*(_size_x-1)+k*(_size_x-1)*(_size_y-1)]==-1)
					{
						_cube_type[i+j*(_size_x-1)+k*(_size_x-1)*(_size_y-1)]=1;


						BoundCubes[_nBoundCubes].i=i;
						BoundCubes[_nBoundCubes].j=j;
						BoundCubes[_nBoundCubes++].k=k;
					}
				}
			}
		}

	}

	outfile<<"bound cubes num="<<_nBoundCubes<<endl;

	for (m=0;m<_nBoundCubes;m++)
	{

		lowx = MAX(0,BoundCubes[m].i-1);
		highx = MIN(_size_x-1,BoundCubes[m].i+1);
		lowy = MAX(0,BoundCubes[m].j-1);
		highy = MIN(_size_y-1,BoundCubes[m].j+1);
		lowz = MAX(0,BoundCubes[m].k-1);
		highz = MIN(_size_z-1,BoundCubes[m].k+1);

		for(i=lowx;i<=highx;i++)
		{
			for (j=lowy;j<=highy;j++)
			{
				for (k=lowz;k<=highz;k++)
				{
					if(_cube_type[i+j*(_size_x-1)+k*(_size_x-1)*(_size_y-1)]==-1)
					{
						vmin.Set(i*step[0],j*step[1],k*step[2]);
						vmin+=StartPos;
						vmax=vmin+step;
						CBox3 box;
						box.Reset(vmin,vmax);
						for(n=0;n<triband.size();n++)
						{
							tri=triband.at(n);
							if(CIntersectionTest::Box_Triangle(box,tri,StartPos,_bb.MaxAxisLength()))
							{
								_cube_type[i+j*(_size_x-1)+k*(_size_x-1)*(_size_y-1)]=2;

								invCubes_Cur[_nInvCubes_Cur].i=i;
								invCubes_Cur[_nInvCubes_Cur].j=j;
								invCubes_Cur[_nInvCubes_Cur++].k=k;
								break;
							}
						}
						//if(n==triband.size())
						//	_cube_type[i+j*(_size_x-1)+k*(_size_x-1)*(_size_y-1)]=0;

					}

				}
			}
		}
	}


	_nInvCubes_Bak=0;
	while (_nInvCubes_Cur)
	{
		for(m=0;m<_nInvCubes_Cur;m++)
		{

			i=invCubes_Cur[m].i+1;
			j=invCubes_Cur[m].j;
			k=invCubes_Cur[m].k;
			if(i<(_size_x-1))
			{
				if(_cube_type[i+j*(_size_x-1)+k*(_size_x-1)*(_size_y-1)]==-1)
				{
					_cube_type[i+j*(_size_x-1)+k*(_size_x-1)*(_size_y-1)]=2;
					invCubes_Bak[_nInvCubes_Bak].i=i;
					invCubes_Bak[_nInvCubes_Bak].j=j;
					invCubes_Bak[_nInvCubes_Bak++].k=k;
				}
			}
			i=invCubes_Cur[m].i-1;
			j=invCubes_Cur[m].j;
			k=invCubes_Cur[m].k;
			if(i>=0)
			{
				if(_cube_type[i+j*(_size_x-1)+k*(_size_x-1)*(_size_y-1)]==-1)
				{
					_cube_type[i+j*(_size_x-1)+k*(_size_x-1)*(_size_y-1)]=2;
					invCubes_Bak[_nInvCubes_Bak].i=i;
					invCubes_Bak[_nInvCubes_Bak].j=j;
					invCubes_Bak[_nInvCubes_Bak++].k=k;
				}
			}

			i=invCubes_Cur[m].i;
			j=invCubes_Cur[m].j+1;
			k=invCubes_Cur[m].k;
			if(j<(_size_y-1))
			{
				if(_cube_type[i+j*(_size_x-1)+k*(_size_x-1)*(_size_y-1)]==-1)
				{
					_cube_type[i+j*(_size_x-1)+k*(_size_x-1)*(_size_y-1)]=2;
					invCubes_Bak[_nInvCubes_Bak].i=i;
					invCubes_Bak[_nInvCubes_Bak].j=j;
					invCubes_Bak[_nInvCubes_Bak++].k=k;
				}

			}
			i=invCubes_Cur[m].i;
			j=invCubes_Cur[m].j-1;
			k=invCubes_Cur[m].k;
			if(j>=0)
			{
				if(_cube_type[i+j*(_size_x-1)+k*(_size_x-1)*(_size_y-1)]==-1)
				{
					_cube_type[i+j*(_size_x-1)+k*(_size_x-1)*(_size_y-1)]=2;
					invCubes_Bak[_nInvCubes_Bak].i=i;
					invCubes_Bak[_nInvCubes_Bak].j=j;
					invCubes_Bak[_nInvCubes_Bak++].k=k;
				}
			}
			i=invCubes_Cur[m].i;
			j=invCubes_Cur[m].j;
			k=invCubes_Cur[m].k+1;
			if(k<(_size_z-1))
			{
				if(_cube_type[i+j*(_size_x-1)+k*(_size_x-1)*(_size_y-1)]==-1)
				{
					_cube_type[i+j*(_size_x-1)+k*(_size_x-1)*(_size_y-1)]=2;
					invCubes_Bak[_nInvCubes_Bak].i=i;
					invCubes_Bak[_nInvCubes_Bak].j=j;
					invCubes_Bak[_nInvCubes_Bak++].k=k;
				}

			}

			i=invCubes_Cur[m].i;
			j=invCubes_Cur[m].j;
			k=invCubes_Cur[m].k-1;
			if(k>=0)
			{
				if(_cube_type[i+j*(_size_x-1)+k*(_size_x-1)*(_size_y-1)]==-1)
				{
					_cube_type[i+j*(_size_x-1)+k*(_size_x-1)*(_size_y-1)]=2;
					invCubes_Bak[_nInvCubes_Bak].i=i;
					invCubes_Bak[_nInvCubes_Bak].j=j;
					invCubes_Bak[_nInvCubes_Bak++].k=k;
				}

			}
		}
		invCubes_Tmp=invCubes_Cur;
		invCubes_Cur=invCubes_Bak;
		invCubes_Bak=invCubes_Tmp;
		_nInvCubes_Cur=_nInvCubes_Bak;
		_nInvCubes_Bak=0;
	}
	delete []invCubes_Cur;
	delete []invCubes_Bak;
	delete []BoundCubes;
	//outfile<<"ok"<<endl;

}


CBox3& MarchingCubes::get_boundingbox(void)
{
	return _bb;
}

void MarchingCubes::GetCube(int i, int j, int k, CBox3& cb, int& ct)
{
	CVector3d StartPos,step,vmin,vmax;

	StartPos=_bb.GetMinCorner();

	step.Set(_bb.XLength()/(_size_x-1),_bb.YLength()/(_size_y-1),_bb.ZLength()/(_size_z-1));

	vmin.Set(i*step[0],j*step[1],k*step[2]);
	vmin+=StartPos;
	vmax=vmin+step;
	cb.Reset(vmin,vmax);

	ct=_cube_type[i+j*(_size_x-1)+k*(_size_x-1)*(_size_y-1)];


}