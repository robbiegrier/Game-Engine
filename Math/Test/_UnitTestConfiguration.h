//-----------------------------------------------------------------------------
// Copyright 2023, Ed Keenan, all rights reserved.
//-----------------------------------------------------------------------------  

#ifndef UNIT_TEST_CONFIGURATION_H
#define UNIT_TEST_CONFIGURATION_H

// Make sure all are enabled for final submission...
//      I grade whatever is set in this file - Keenan

#define Vec3App_Lerp                            true
#define Vec3App_Lerp2                           true
#define Vec3App_Lerp3                           true
#define Vec3App_Lerp4                           true

#define Vec3App_LERP_Array                      true
#define Vec3App_LERP_Array0                     true
#define Vec3App_LERP_Array1                     true

#define Combo_Junkfood                          true                                   
#define Combo_VectCrossAddSub                   true                            
#define Combo_VectCrossAddSubMatrixMult         true                  
#define Combo_VectCrossAddSubMatrixMultCross    true             
#define Combo_VectCrossCrossNorm                true                         
#define Combo_VectLine                          true      

#define Mat3_Accessor_Get                       true                                
#define Mat3_Accessor_Get_Const                 true                          
#define Mat3_Accessor_Set                       true                                
#define Mat3_AddEqual_Mat3                      true                             
#define Mat3_Add_Mat3                           true                                  
#define Mat3_AssignmentOperator                 true                          
#define Mat3_Bracket_Get                        true                                 
#define Mat3_Bracket_Get_Const                  true                           
#define Mat3_Bracket_Set                        true                                 
#define Mat3_Copy_Constructor                   true                            
#define Mat3_Default_Constructor                true                         
#define Mat3_Destructor_Constuctor              true                       
#define Mat3_Det                                true  
#define Mat3_GetInverse                         true                                  
#define Mat3_GetTranspose                       true                                
#define Mat3_GET_ROW_0                          true                                   
#define Mat3_GET_ROW_1                          true                                   
#define Mat3_GET_ROW_2                          true                                                                    
#define Mat3_Inverse                            true                                     
#define Mat3_isEqual_false                      true                               
#define Mat3_MultEqual_Mat3                     true                              
#define Mat3_Mult_Mat3                          true                          
#define Mat3_Mult_Scale                         true                                
#define Mat3_Scale_Equals                       true                                
#define Mat3_Scale_Mat3                         true                                
#define Mat3_SET_Identity                       true                                                            
#define Mat3_SET_ROW_0                          true                                   
#define Mat3_SET_ROW_1                          true                                   
#define Mat3_SET_ROW_2                          true                                                                                                 
#define Mat3_SET_Vectors                        true                                 
#define Mat3_SET_Zero                           true
#define Mat3_Special_isEqual_false2             true                      
#define Mat3_Special_isEqual_true               true                        
#define Mat3_Special_isIdentity_false           true                    
#define Mat3_Special_isIdentity_true            true                     
#define Mat3_Special_Matrix_Identity            true                                       
#define Mat3_Special_Matrix_Zero                true                                                        
#define Mat3_SubEqual_Mat3                      true                             
#define Mat3_Sub_Mat3                           true                                  
#define Mat3_Transpose                          true                                                                      
#define Mat3_UnaryMinus                         true                                
#define Mat3_UnaryPlus                          true                                 
#define Mat3_Unary_equal_Minus                  true                         
#define Mat3_Unary_equal_Plus                   true   
#define Mat3_Vector_Constructor                 true   

#define Mat4_Accessor_Get                       true                                
#define Mat4_Accessor_Get_Const                 true                          
#define Mat4_Accessor_Set                       true                                
#define Mat4_AddEqual_Mat4                      true                             
#define Mat4_Add_Mat4                           true                                  
#define Mat4_AssignmentOperator                 true                          
#define Mat4_Bracket_Get                        true                                 
#define Mat4_Bracket_Get_Const                  true                           
#define Mat4_Bracket_Set                        true                                 
#define Mat4_Copy_Constructor                   true                            
#define Mat4_Default_Constructor                true                         
#define Mat4_Destructor_Constuctor              true                       
#define Mat4_Det                                true                                         
#define Mat4_GetInverse                         true                                  
#define Mat4_GetTranspose                       true                                
#define Mat4_GET_ROW_0                          true                                   
#define Mat4_GET_ROW_1                          true                                   
#define Mat4_GET_ROW_2                          true                                   
#define Mat4_GET_ROW_3                          true                                   
#define Mat4_Inverse                            true                                     
#define Mat4_isEqual_false                      true                                                           
#define Mat4_MultEqual_Mat4                     true                              
#define Mat4_Mult_Mat4                          true                          
#define Mat4_Mult_Scale                         true                                
#define Mat4_RotAxisAngle                       true  
                              
#define Mat4_RotOrient                          true                                   
#define Mat4_Scale_Equals                       true                                
#define Mat4_Scale_Mat4                         true                                
#define Mat4_SET_Identity                       true                                
#define Mat4_SET_MatrixRotAxisAngle             true                      

#define Mat4_SET_MatrixRotOrient                true                         
#define Mat4_SET_RotX                           true                                    
#define Mat4_SET_RotXYZ                         true                                  
#define Mat4_SET_RotY                           true                                    
#define Mat4_SET_RotZ                           true                                    
#define Mat4_SET_ROW_0                          true                                   
#define Mat4_SET_ROW_1                          true                                   
#define Mat4_SET_ROW_2                          true                                   
#define Mat4_SET_ROW_3                          true                                   
#define Mat4_SET_SCALE                          true                                   
#define Mat4_SET_SCALE_Vect                     true                              
#define Mat4_SET_Trans                          true                                   
#define Mat4_SET_Trans_Vect                     true                              
#define Mat4_SET_Vectors                        true                                 
#define Mat4_SET_Zero                           true                                    
#define Mat4_Special_isEqual_false2             true                      
#define Mat4_Special_isEqual_true               true                        
#define Mat4_Special_isIdentity_false           true                    
#define Mat4_Special_isIdentity_true            true                     
#define Mat4_Special_Matrix_Identity            true                     
#define Mat4_Special_Matrix_SCALE               true                        
#define Mat4_Special_Matrix_SCALE_Vect          true                   
#define Mat4_Special_Matrix_Trans               true                        
#define Mat4_Special_Matrix_Trans_Vect          true                   
#define Mat4_Special_Matrix_Zero                true                         
#define Mat4_Special_RotX                       true                                
#define Mat4_Special_RotXYZ_Constructor         true                  
#define Mat4_Special_RotY                       true                                
#define Mat4_Special_RotZ                       true                                
#define Mat4_SubEqual_Mat4                      true                             
#define Mat4_Sub_Mat4                           true                                  
#define Mat4_Transpose                          true                                   
#define Mat4_UnaryMinus                         true                                
#define Mat4_UnaryPlus                          true                                 
#define Mat4_Unary_equal_Minus                  true                         
#define Mat4_Unary_equal_Plus                   true   
#define Mat4_Vector_Constructor                 true       
									            
#define Trig_ACos                               true                                        
#define Trig_ASin                               true                                        
#define Trig_ATan                               true                                        
#define Trig_ATan2                              true                                       
#define Trig_Cos                                true                                         
#define Trig_CosSin                             true                                      
#define Trig_InvSqrt                            true                                     
#define Trig_Sin                                true                                         
#define Trig_Sqrt                               true                                        
#define Trig_Tan                                true    

#define Vec3_Add_Vec3                           true                                    
#define Vec3_Assignment_Operator                true     
#define Vec3_Assignment_Operator_Vec4           true
#define Vec3_ConstantCheck                      true                               
#define Vec3_Constructor_Copy                   true                            
#define Vec3_Constructor_Default                true                         
#define Vec3_Constructor_Individual             true                       
#define Vec3_Constructor_Vec4                   true                             
#define Vec3_Cross                              true                                       
#define Vec3_Destructor                         true                                  
#define Vec3_Dot                                true                                         
#define Vec3_Get_Accessors                      true                               
#define Vec3_Get_Accessors_const                true                         
#define Vec3_Get_Angle                          true                                   
#define Vec3_Get_Brackets                       true                                
#define Vec3_Get_Brackets_const                 true                          
#define Vec3_Get_Normalize                      true                               
#define Vec3_IsEqualFalse                       true                                
#define Vec3_IsEqualFalse_default               true                        
#define Vec3_IsEqualTrue                        true                                 
#define Vec3_IsEqualTrue_default                true                         
#define Vec3_IsZeroFalse                        true                                 
#define Vec3_IsZeroFalse_default                true                         
#define Vec3_IsZeroTrue_default                 true                          
#define Vec3_IsZeroTrue_True                    true                             
#define Vec3_Len                                true                                         
#define Vec3_MultEqual_Mat3                     true                              
#define Vec3_MultEqual_Mat4                     true                              
#define Vec3_Mult_Mat3                          true                                   
#define Vec3_Mult_Mat4                          true                                   
#define Vec3_Mult_Scale                         true                                  
#define Vec3_Normalize                          true                                   
#define Vec3_PlusEqual_Vect                     true                              
#define Vec3_Proxy                              true                                       
#define Vec3_SampleTest                         true                                  
#define Vec3_ScaleEquals                        true                                 
#define Vec3_Scale_Vect                         true                                  
#define Vec3_Set_3_Elements                     true                              
#define Vec3_Set_Accessors                      true                               
#define Vec3_Set_Accessors_get                  true                           
#define Vec3_Set_Brackets                       true                                
#define Vec3_Set_Brackets_get                   true                            
#define Vec3_Set_Vec3                           true                                    
#define Vec3_Set_Vec4                           true                                    
#define Vec3_SubEqual_Vect                      true                               
#define Vec3_Sub_Vect                           true                                    
#define Vec3_Unary_equal_Minus                  true                           
#define Vec3_Unary_equal_Plus                   true                            
#define Vec3_Unary_Minus                        true                                 
#define Vec3_Unary_Plus                         true    
									            
#define Vec4_Add_Vec4                           true                                    
#define Vec4_Assignment_Operator                true                         
#define Vec4_ConstantCheck                      true                               
#define Vec4_Constructor_Copy                   true  
#define Vec4_Constructor_Default                true                            
#define Vec4_Constructor_Individual             true                      
#define Vec4_Constructor_Vec3                   true
#define Vec4_Destructor                         true                                  
#define Vec4_Dot                                true                                         
#define Vec4_Get_Accessors                      true                               
#define Vec4_Get_Accessors_const                true                         
#define Vec4_Get_Brackets                       true                                
#define Vec4_Get_Brackets_const                 true                          
#define Vec4_Get_Normalize                      true                               
#define Vec4_IsEqualFalse                       true                                
#define Vec4_IsEqualFalse_default               true                        
#define Vec4_IsEqualTrue                        true                                 
#define Vec4_IsEqualTrue_default                true                         
#define Vec4_IsZeroFalse                        true                                 
#define Vec4_IsZeroFalse_default                true                         
#define Vec4_IsZeroTrue_default                 true                          
#define Vec4_IsZeroTrue_True                    true                             
#define Vec4_Len                                true                                         
#define Vec4_MultEqual_Mat4                     true                              
#define Vec4_Mult_Mat4                          true                                   
#define Vec4_Mult_Scale                         true                                  
#define Vec4_Normalize                          true                                   
#define Vec4_PlusEqual_Vec4                     true                              
#define Vec4_Proxy                              true                                       
#define Vec4_SampleTest                         true                                  
#define Vec4_ScaleEquals                        true                                 
#define Vec4_Scale_Vec4                         true                                  
#define Vec4_Set_4_Elements                     true                              
#define Vec4_Set_Accessors                      true                               
#define Vec4_Set_Accessors_Get                  true                           
#define Vec4_Set_Brackets                       true                                
#define Vec4_Set_Brackets_get                   true                            
#define Vec4_Set_Vec3                           true                                    
#define Vec4_Set_Vec4                           true                                    
#define Vec4_SubEqual_Vec4                      true                               
#define Vec4_Sub_Vec4                           true                                    
#define Vec4_Unary_equal_Minus                  true                           
#define Vec4_Unary_equal_Plus                   true                            
#define Vec4_Unary_Minus                        true                                 
#define Vec4_Unary_Plus                         true                                  

		     



// ---  End of File ---------------





#endif

// ---  End of File ---------------
