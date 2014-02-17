/*
 *    This file is part of ACADO Toolkit.
 *
 *    ACADO Toolkit -- A Toolkit for Automatic Control and Dynamic Optimization.
 *    Copyright (C) 2008-2014 by Boris Houska, Hans Joachim Ferreau,
 *    Milan Vukov, Rien Quirynen, KU Leuven.
 *    Developed within the Optimization in Engineering Center (OPTEC)
 *    under supervision of Moritz Diehl. All rights reserved.
 *
 *    ACADO Toolkit is free software; you can redistribute it and/or
 *    modify it under the terms of the GNU Lesser General Public
 *    License as published by the Free Software Foundation; either
 *    version 3 of the License, or (at your option) any later version.
 *
 *    ACADO Toolkit is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *    Lesser General Public License for more details.
 *
 *    You should have received a copy of the GNU Lesser General Public
 *    License along with ACADO Toolkit; if not, write to the Free Software
 *    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 *
 */



/**
*    \file src/symbolic_operator/asin.cpp
*    \author Boris Houska, Hans Joachim Ferreau
*    \date 2008
*/


#include <acado/utils/acado_utils.hpp>
#include <acado/symbolic_operator/symbolic_operator.hpp>


double dAsin(double x){
  return 1/sqrt(1-x*x);
}

double ddAsin(double x){
  double v1 = sqrt(1-x*x);
  return -2*x*(-0.5/v1/v1/v1);
}

BEGIN_NAMESPACE_ACADO


Asin::Asin():UnaryOperator(){
  cName = "asin";

  fcn = &asin;
  dfcn = &dAsin;
  ddfcn = &ddAsin;

  operatorName = ON_ASIN;

}

Asin::Asin( Operator *_argument ):UnaryOperator(_argument){
  cName = "asin";

  fcn = &asin;
  dfcn = &dAsin;
  ddfcn = &ddAsin;

  operatorName = ON_ASIN;
}


Asin::Asin( const Asin &arg ):UnaryOperator(arg){
  cName = "asin";

  fcn = &asin;
  dfcn = &dAsin;
  ddfcn = &ddAsin;

  operatorName = ON_ASIN;
}


Asin::~Asin(){

}


Asin& Asin::operator=( const Asin &arg ){

  UnaryOperator::operator=(arg);

  return *this;
}


returnValue Asin::evaluate( EvaluationBase *x ){
 
    x->Asin(*argument);
    return SUCCESSFUL_RETURN;
}


Operator* Asin::differentiate( int index ){

  dargument = argument->differentiate( index );
  if( dargument->isOneOrZero() == NE_ZERO ){
    return new DoubleConstant( 0.0 , NE_ZERO );
  }
  if( dargument->isOneOrZero() == NE_ONE ){
    return new Power(
                   new Subtraction(
                           new DoubleConstant(1.0 , NE_ONE),
                           new Power_Int(
                                   argument->clone(),
                                   2
                               )
                       ),
                   new DoubleConstant( -0.5 , NE_NEITHER_ONE_NOR_ZERO )
              );
  }
  return new Product(
                 new Power(
                         new Subtraction(
                                 new DoubleConstant(1.0 , NE_ONE),
                                 new Power_Int(
                                         argument->clone(),
                                         2
                                     )
                             ),
                         new DoubleConstant( -0.5 , NE_NEITHER_ONE_NOR_ZERO )
                     ),
                 dargument->clone()
            );
}



Operator* Asin::ADforwardProtected( int dim,
                                      VariableType *varType,
                                      int *component,
                                      Operator **seed,
                                      int &nNewIS,
                                      TreeProjection ***newIS  ){

    if( dargument != 0 )
        delete dargument;

    dargument = argument->AD_forward(dim,varType,component,seed,nNewIS,newIS);

    if( dargument->isOneOrZero() == NE_ZERO ){
        return new DoubleConstant( 0.0 , NE_ZERO );
    }
    if( dargument->isOneOrZero() == NE_ONE ){
        return new Power(
                   new Subtraction(
                           new DoubleConstant(1.0 , NE_ONE),
                           new Power_Int(
                                   argument->clone(),
                                   2
                               )
                       ),
                   new DoubleConstant( -0.5 , NE_NEITHER_ONE_NOR_ZERO )
             );
    }
    return new Product(
                 new Power(
                         new Subtraction(
                                 new DoubleConstant(1.0 , NE_ONE),
                                 new Power_Int(
                                         argument->clone(),
                                         2
                                     )
                             ),
                         new DoubleConstant( -0.5 , NE_NEITHER_ONE_NOR_ZERO )
                     ),
                 dargument->clone()
         );
}



returnValue Asin::ADbackwardProtected( int           dim      , /**< number of directions  */
                                        VariableType *varType  , /**< the variable types    */
                                        int          *component, /**< and their components  */
                                        Operator     *seed     , /**< the backward seed     */
                                        Operator    **df       , /**< the result            */
                                        int           &nNewIS  , /**< the number of new IS  */
                                        TreeProjection ***newIS  /**< the new IS-pointer    */ ){


    if( seed->isOneOrZero() == NE_ZERO ){
            argument->AD_backward( dim,
                                          varType,
                                          component,
                                          new DoubleConstant( 0.0 , NE_ZERO ),
                                          df, nNewIS, newIS
            );
        delete seed;
        return SUCCESSFUL_RETURN;
    }
    if( seed->isOneOrZero() == NE_ONE ){
            argument->AD_backward( dim,
                                          varType,
                                          component,
                              new Power(
                                  new Subtraction(
                                      new DoubleConstant(1.0 , NE_ONE),
                                      new Power_Int(
                                          argument->clone(),
                                          2
                                      )
                                  ),
                                  new DoubleConstant( -0.5 , NE_NEITHER_ONE_NOR_ZERO )
                              ),
                              df, nNewIS, newIS
            );
        delete seed;
        return SUCCESSFUL_RETURN;
    }
    argument->AD_backward( dim,
                                  varType,
                                  component,
                                  new Product(
                                      new Power(
                                          new Subtraction(
                                              new DoubleConstant(1.0 , NE_ONE),
                                              new Power_Int(
                                                  argument->clone(),
                                                  2
                                              )
                                          ),
                                          new DoubleConstant( -0.5 , NE_NEITHER_ONE_NOR_ZERO )
                                      ),
                                      seed->clone()
                                  ),
                                  df, nNewIS, newIS
            );

    delete seed;
    return SUCCESSFUL_RETURN;
}


returnValue Asin::ADsymmetricProtected( int            dim       , /**< number of directions  */
                                        VariableType  *varType   , /**< the variable types    */
                                        int           *component , /**< and their components  */
                                        Operator      *l         , /**< the backward seed     */
                                        Operator     **S         , /**< forward seed matrix   */
                                        int            dimS      , /**< dimension of forward seed             */
                                        Operator     **dfS       , /**< first order foward result             */
                                        Operator     **ldf       , /**< first order backward result           */
                                        Operator     **H         , /**< upper trianglular part of the Hessian */
                                      int            &nNewLIS  , /**< the number of newLIS  */
                                      TreeProjection ***newLIS , /**< the new LIS-pointer   */
                                      int            &nNewSIS  , /**< the number of newSIS  */
                                      TreeProjection ***newSIS , /**< the new SIS-pointer   */
                                      int            &nNewHIS  , /**< the number of newHIS  */
                                      TreeProjection ***newHIS   /**< the new HIS-pointer   */ ){
  
    TreeProjection tmp, tmp2;
    tmp  = Power(
		         new Addition(
                            new DoubleConstant(1.0 , NE_ONE),
                            new Product(
                                new DoubleConstant( -1.0, NE_NEITHER_ONE_NOR_ZERO),
                                new Power_Int(
                                    argument->clone(),
                                    2
                                )
                            )
                         ),
                         new DoubleConstant( -0.5 , NE_NEITHER_ONE_NOR_ZERO )
               );    
    
    tmp2 = Product(
		         new Power(
		           new Addition(
                            new DoubleConstant(1.0 , NE_ONE),
                            new Product(
                                new DoubleConstant( -1.0, NE_NEITHER_ONE_NOR_ZERO),
                                new Power_Int(
                                    argument->clone(),
                                    2
                                )
                            )
                           ),
                           new DoubleConstant( -1.5 , NE_NEITHER_ONE_NOR_ZERO )
                        ),
		         argument->clone()
           );
    
    return ADsymCommon( argument, tmp, tmp2, dim, varType, component, l, S, dimS, dfS,
			 ldf, H, nNewLIS, newLIS, nNewSIS, newSIS, nNewHIS, newHIS );
}



Operator* Asin::substitute( int index, const Operator *sub ){

    return new Asin( argument->substitute( index , sub ) );

}

Operator* Asin::clone() const{

    return new Asin(*this);
}


CLOSE_NAMESPACE_ACADO

// end of file.
