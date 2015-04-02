/**********************************************/
/* ����:                                      */
/*      ���������йصĺ���                        */
/* ����:                                      */
/*      ����                                  */
/**********************************************/
#include <base.h>

extern double tInterval;			/* �����̬�����Ĳ��� */
extern const double ae;			/* ����İ뾶 */
extern const double GM;			/* ������������ */
extern const double J2; 			/* J2�㶯�� */


/* �����Ϣ�ں� */
void orbInfoCom(double orbInfo[6],double J2000PV[6])
{/*�ںϷ���Ϊȡ����ֵ�����ֵ�ľ�ֵ*/

 orbInfo[0] = 0.5*(orbInfo[0] +J2000PV[0]);
 orbInfo[1] = 0.5*(orbInfo[1] +J2000PV[1]);
 orbInfo[2] = 0.5*(orbInfo[2] +J2000PV[2]);
 orbInfo[3] = 0.5*(orbInfo[3] +J2000PV[3]);
 orbInfo[4] = 0.5*(orbInfo[4] +J2000PV[4]);
 orbInfo[5] = 0.5*(orbInfo[5] +J2000PV[5]);
     
   return;
}


/*--------------------------------------*/
/* ������� */
void orbPro(double orbInfo[6],double orbInc[3])
{ /*�㷨��Ʋμ������ϸ��Ʊ���*/
    double orbTmp[6],orbit[6];
    double orbk[4][6],orb[6];
    double radius,tmp,a;
    
    mtxCpy(orbit,orbInfo,1,6);
    
    mtxCpy(orbTmp,orbit,1,6);
    mtxMtp(orbk[0],&orbTmp[3],3,1,&tInterval,1,1);

    radius = norm(orbTmp,3);
    tmp = GM/radius/radius*ae/radius*ae/radius*J2*(7.5*(orbTmp[2]/radius)*(orbTmp[2]/radius) - 1.5)/radius;

    orbk[0][3] = -GM*orbTmp[0]/(radius*radius*radius) + tmp*orbTmp[0];
    orbk[0][4] = -GM*orbTmp[1]/(radius*radius*radius) + tmp*orbTmp[1];	
    tmp = GM/radius/radius*ae/radius*ae/radius*J2*(7.5*(orbTmp[2]/radius)*(orbTmp[2]/radius) - 4.5)/radius;
    orbk[0][5] = -GM*orbTmp[2]/(radius*radius*radius) + tmp*orbTmp[2];
    
    mtxMtp(&orbk[0][3],&orbk[0][3],1,3,&tInterval,1,1);
       		               		/* RK ��һ�� */
    a = 0.5;
    mtxMtp(orb,orbk[0],1,6,&a,1,1);
    mtxAdd(orbTmp,orbit,orb,1,6);
    mtxMtp(orbk[1],&orbTmp[3],3,1,&tInterval,1,1);

    radius = norm(orbTmp,3);
    tmp = GM/radius/radius*ae/radius*ae/radius*J2*(7.5*(orbTmp[2]/radius)*(orbTmp[2]/radius) - 1.5)/radius;
    orbk[1][3] = -GM*orbTmp[0]/(radius*radius*radius) + tmp*orbTmp[0];
    orbk[1][4] = -GM*orbTmp[1]/(radius*radius*radius) + tmp*orbTmp[1];	
    tmp = GM/radius/radius*ae/radius*ae/radius*J2*(7.5*(orbTmp[2]/radius)*(orbTmp[2]/radius) - 4.5)/radius;
    orbk[1][5] = -GM*orbTmp[2]/(radius*radius*radius) + tmp*orbTmp[2];
    mtxMtp(&orbk[1][3],&orbk[1][3],1,3,&tInterval,1,1);
       		              		/* RK �ڶ��� */
    mtxMtp(orb,orbk[1],1,6,&a,1,1);
    mtxAdd(orbTmp,orbit,orb,1,6);
    mtxMtp(orbk[2],&orbTmp[3],3,1,&tInterval,1,1);
    radius = norm(orbTmp,3);
    tmp = GM/radius/radius*ae/radius*ae/radius*J2*(7.5*(orbTmp[2]/radius)*(orbTmp[2]/radius) - 1.5)/radius;
    orbk[2][3] = -GM*orbTmp[0]/(radius*radius*radius) + tmp*orbTmp[0];
    orbk[2][4] = -GM*orbTmp[1]/(radius*radius*radius) + tmp*orbTmp[1];	
    tmp = GM/radius/radius*ae/radius*ae/radius*J2*(7.5*(orbTmp[2]/radius)*(orbTmp[2]/radius) - 4.5)/radius;
    orbk[2][5] = -GM*orbTmp[2]/(radius*radius*radius) + tmp*orbTmp[2];
    mtxMtp(&orbk[2][3],&orbk[2][3],1,3,&tInterval,1,1);
       		              		/* RK ������ */
       
    mtxAdd(orbTmp,orbit,orbk[2],1,6);
    mtxMtp(orbk[3],&orbTmp[3],3,1,&tInterval,1,1);
    radius = norm(orbTmp,3);
    tmp = GM/radius/radius*ae/radius*ae/radius*J2*(7.5*(orbTmp[2]/radius)*(orbTmp[2]/radius) - 1.5)/radius;

    orbk[3][3] = -GM*orbTmp[0]/(radius*radius*radius) + tmp*orbTmp[0];
    orbk[3][4] = -GM*orbTmp[1]/(radius*radius*radius) + tmp*orbTmp[1];	
    tmp = GM/radius/radius*ae/radius*ae/radius*J2*(7.5*(orbTmp[2]/radius)*(orbTmp[2]/radius) - 4.5)/radius;
    orbk[3][5] = -GM*orbTmp[2]/(radius*radius*radius) + tmp*orbTmp[2];
    mtxMtp(&orbk[3][3],&orbk[3][3],1,3,&tInterval,1,1);

       		           		/* RK ���Ĳ� */
    mtxAdd(orb,orbk[0],orbk[1],1,6);
    mtxAdd(orb,orb,orbk[2],1,6);
    mtxAdd(orb,orb,orbk[3],1,6);
    mtxAdd(orb,orb,orbk[1],1,6);
    mtxAdd(orb,orb,orbk[2],1,6);
    a = 1.0/6.0;
    mtxMtp(orb,orb,1,6,&a,1,1);
    mtxAdd(orbit,orbit,orb,1,6);

    mtxSub(orbInc,orbit,orbInfo,1,3);
    mtxCpy(orbInfo,orbit,1,6);

       
    return;
}


