#include "../hedder/codetimer.h"
#include "../hedder/dark_compiler.h"

int precomputation()
{
	
	return 1;
}

int commit_new_precom(_struct_commit_* cm, _struct_pp_ pp, _struct_poly_ poly)
{
	unsigned long long int RunTime_file_IO = 0, RunTime_commit = 0;

	int flag = 1, i = 0, j = 0;
	qfb_t qfb_tmp;

	qfb_init(qfb_tmp);
	qfb_principal_form(cm->C, pp.G);
	
	/////pre computation
	static int isfirst = 1;
	static qfb_t** qfb_q_pt;
	static int d;

	if(isfirst || poly.d > d)
	{
		TimerOff();
		printf("Start precomputation\n");
		TimerOn();
		d = poly.d + 1;
		if(isfirst == 0)
		{
			for(i=0; i<d; i++)
			{
				//qfb_clear(qfb_q_pt[i]);
			}
		}
		else
			isfirst = 0;

		(qfb_q_pt) = (qfb_t**)calloc(sizeof(qfb_t*), d);
		for(i=0; i<d; i++)
			(qfb_q_pt[i]) = (qfb_t*)calloc(sizeof(qfb_t), fmpz_bits(pp.q));

		
		qfb_init(qfb_q_pt[0][0]);
		qfb_set(qfb_q_pt[0][0], pp.g);

		for(i=1; i<d; i++)
		{
			qfb_init(qfb_q_pt[i][0]);
			qfb_pow_with_root(qfb_q_pt[i][0], qfb_q_pt[i-1][0], pp.G, pp.q, pp.L);
			qfb_reduce(qfb_q_pt[i][0], qfb_q_pt[i][0], pp.G);
			//printf("%4.2lf%%\n", 100.0*(i)/(d));
		}

		for(i=0; i<d; i++)
		{
			for(j=1; j < fmpz_bits(pp.q); j++)
			{
				qfb_init(qfb_q_pt[i][j]);
				qfb_nucomp(qfb_q_pt[i][j], qfb_q_pt[i][j-1], qfb_q_pt[i][j-1], pp.G, pp.L);
				qfb_reduce(qfb_q_pt[i][j], qfb_q_pt[i][j], pp.G);
				//printf("%4.2lf%%\n", 100.0*(fmpz_bits(pp.q)*i+j)/(fmpz_bits(pp.q)*d));
			}
		}

		RunTime_commit = TimerOff();
		printf("Commit__PRE_ %12llu [us]\n", RunTime_commit);
		TimerOn();
	}

	//////////
	for(i = poly.d - 1; i >= 0; i--)
	{
		qfb_principal_form(qfb_tmp, pp.G);
		//printf("%d\n", i);
		for(j=0; j< fmpz_bits(poly.Fx[i]); j++)
		{
			if(fmpz_tstbit(poly.Fx[i], j) == 1)
			{
				qfb_nucomp(qfb_tmp, qfb_tmp, qfb_q_pt[i][j], pp.G, pp.L);
				qfb_reduce(qfb_tmp, qfb_tmp, pp.G);				
			}
		}
		//qfb_pow_with_root(qfb_tmp, qfb_q_pt[i], pp.G, poly.Fx[i], pp.L);
		qfb_nucomp(cm->C, cm->C, qfb_tmp, pp.G, pp.L);
		qfb_reduce(cm->C, cm->C, pp.G);
	}

	//printf("\n");
	qfb_clear(qfb_tmp);

	return flag;
}

int commit_new(_struct_commit_* cm, _struct_pp_ pp, _struct_poly_ poly)
{
	unsigned long long int RunTime_file_IO = 0, RunTime_commit = 0;

	int flag = 1, i = 0;
	qfb_t qfb_tmp;

	qfb_init(qfb_tmp);
	qfb_principal_form(cm->C, pp.G);
	
	/////pre computation
	static int isfirst = 1;
	static qfb_t* qfb_q_pt;
	static int d;

	if(isfirst || poly.d > d)
	{
		TimerOff();
		printf("Start precomputation\n");
		TimerOn();
		d = poly.d + 5;
		if(isfirst == 0)
		{
			for(i=0; i<d; i++)
			{
				qfb_clear(qfb_q_pt[i]);
			}
		}
		else
			isfirst = 0;
		(qfb_q_pt) = (qfb_t*)calloc(sizeof(qfb_t), d);

		qfb_init(qfb_q_pt[0]);
		qfb_set(qfb_q_pt[0], pp.g);

		for(i=1; i<d; i++)
		{
			qfb_init(qfb_q_pt[i]);
			qfb_pow_with_root(qfb_q_pt[i], qfb_q_pt[i-1], pp.G, pp.q, pp.L);
			qfb_reduce(qfb_q_pt[i], qfb_q_pt[i], pp.G);
		}

		RunTime_commit = TimerOff();
		printf("Commit__PRE_ %12llu [us]\n", RunTime_commit);
		TimerOn();
	}

	//////////
	for(i = poly.d - 1; i >= 0; i--)
	{
		// printf("%d %d %d %d\r\n", (int)fmpz_bits(qfb_tmp->a),
		// 	(int)fmpz_bits(qfb_tmp->b),
		// 	(int)fmpz_bits(qfb_tmp->c),
		// 	(int)fmpz_bits(poly.Fx[i])
		// );
		qfb_pow_with_root(qfb_tmp, qfb_q_pt[i], pp.G, poly.Fx[i], pp.L);
		qfb_nucomp(cm->C, cm->C, qfb_tmp, pp.G, pp.L);
		qfb_reduce(cm->C, cm->C, pp.G);
	}
	//printf("\n");
	qfb_clear(qfb_tmp);

	return flag;
}



int commit_new_old(_struct_commit_* cm, _struct_pp_ pp, _struct_poly_ poly)
{
	int flag = 1, i = 0;
	qfb_t qfb_tmp;

	qfb_init(qfb_tmp);
	qfb_principal_form(cm->C, pp.G);

	for(i = poly.d - 1; i >= 0; i--)
	{
		//printf("%d\n", i);
		qfb_pow_with_root(cm->C, cm->C, pp.G, pp.q, pp.L);
		qfb_pow_with_root(qfb_tmp, pp.g, pp.G, poly.Fx[i], pp.L);
		qfb_nucomp(cm->C, cm->C, qfb_tmp, pp.G, pp.L);
		qfb_reduce(cm->C, cm->C, pp.G);
	}
	qfb_clear(qfb_tmp);

	return flag;
}

int commit_init(_struct_commit_* cm){
	qfb_init(cm->C);
	return 1;
}
int commit_clear(_struct_commit_* cm){
	qfb_clear(cm->C);
	return 1;
}
