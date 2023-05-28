#include "psetanal.hpp"
#include "utilmain.hpp"
#include <map>
#include <set>
#include <algorithm>

MtcPartCategoryInfo::MtcPartCategoryInfo()
{
	this->Clear();
}

MtcPartCategoryInfo::MtcPartCategoryInfo(MtcPartCategoryInfo &_info)
{
	*this = _info;
}

MtcPartCategoryInfo::~MtcPartCategoryInfo()
{
}

MtcPartCategoryInfo &MtcPartCategoryInfo::operator=(MtcPartCategoryInfo &_info)
{
	if (this != &_info)
	{
		this->size = _info.size;
		this->primary_shape = _info.primary_shape;
		this->avg_ext_profile_ratio = _info.avg_ext_profile_ratio;
		this->avg_part_area_ratio = _info.avg_part_area_ratio;
		this->area_ratio = _info.area_ratio;
		this->fill_area_ratio1 = _info.fill_area_ratio1;
		this->fill_area_ratio2 = _info.fill_area_ratio2;
		this->quantity_ratio = _info.quantity_ratio;
		this->total_region_area = _info.total_region_area;
		this->total_part_area = _info.total_part_area;
		this->total_ext_profile_area = _info.total_ext_profile_area;
		this->total_concavity_area = _info.total_concavity_area;
		this->total_int_profile_area = _info.total_int_profile_area;
		this->total_fill_area1 = _info.total_fill_area1;
		this->total_fill_area2 = _info.total_fill_area2;
		this->total_quantity = _info.total_quantity;
		this->total_parts = _info.total_parts;
	}

	return (*this);
}

MTC_PART_CATEGORY MtcPartCategoryInfo::Size()
{
	return (this->size);
}

MTC_NEST_SHAPE_TYPE MtcPartCategoryInfo::PrimaryShape()
{
	return (this->primary_shape);
}

double MtcPartCategoryInfo::AvgExtProfileRatio()
{
	return (this->avg_ext_profile_ratio);
}

double MtcPartCategoryInfo::AvgPartAreaRatio()
{
	return (this->avg_part_area_ratio);
}

double MtcPartCategoryInfo::AreaRatio()
{
	return (this->area_ratio);
}

double MtcPartCategoryInfo::FillAreaRatio1()
{
	return (this->fill_area_ratio1);
}

double MtcPartCategoryInfo::FillAreaRatio2()
{
	return (this->fill_area_ratio2);
}

double MtcPartCategoryInfo::QuantityRatio()
{
	return (this->quantity_ratio);
}

void MtcPartCategoryInfo::Size(MTC_PART_CATEGORY _size)
{
	this->size = _size;
}

void MtcPartCategoryInfo::PrimaryShape(MTC_NEST_SHAPE_TYPE _primary_shape)
{
	this->primary_shape = _primary_shape;
}

void MtcPartCategoryInfo::AvgExtProfileRatio(double _avg_ext_profile_ratio)
{
	this->avg_ext_profile_ratio = _avg_ext_profile_ratio;
}

void MtcPartCategoryInfo::AvgPartAreaRatio(double _avg_part_area_ratio)
{
	this->avg_part_area_ratio = _avg_part_area_ratio;
}

void MtcPartCategoryInfo::AreaRatio(double _area_ratio)
{
	this->area_ratio = _area_ratio;
}

void MtcPartCategoryInfo::FillAreaRatio1(double _fill_area_ratio1)
{
	this->fill_area_ratio1 = _fill_area_ratio1;
}

void MtcPartCategoryInfo::FillAreaRatio2(double _fill_area_ratio2)
{
	this->fill_area_ratio2 = _fill_area_ratio2;
}

void MtcPartCategoryInfo::QuantityRatio(double _quantity_ratio)
{
	this->quantity_ratio = _quantity_ratio;
}

void MtcPartCategoryInfo::Clear()
{
	this->size = MPC_UNKNOWN;
	this->avg_ext_profile_ratio = 0.0;
	this->avg_part_area_ratio = 0.0;
	this->area_ratio = 0.0;
	this->fill_area_ratio1 = 0.0;
	this->fill_area_ratio2 = 0.0;
	this->quantity_ratio = 0.0;
	this->total_region_area = 0.0;
	this->total_part_area = 0.0;
	this->total_ext_profile_area = 0.0;
	this->total_concavity_area = 0.0;
	this->total_int_profile_area = 0.0;
	this->total_fill_area1 = 0.0;
	this->total_fill_area2 = 0.0;
	this->total_quantity = 0;
	this->total_parts = 0;
	this->primary_shape = NS_UNKNOWN;
}

int MtcPartCategoryInfo::AddPart(MtcClusterScore *_cscore)
{
	int
		error(-1);

	if (_cscore)
	{
		CLUSTER
			*cluster(_cscore->Cluster());
		MtcClusterState
			cState;
		int
			quantity(_cscore->EstimatedFootprintQuantity());
		NEST_PART
			*nestpart;
		PART_LIST_PART
			*part;

		error = 0;
		this->total_quantity += quantity;
		this->total_parts++;
		this->total_region_area += quantity * cluster->get_region().Area();
		nestpart = cluster->get_first_part(&cState);
		while (nestpart)
		{
			part = nestpart->get_part();
			this->total_part_area += (quantity * part->get_area());
			this->total_ext_profile_area += (quantity * part->ExteriorProfileArea());
			this->total_concavity_area += (quantity * this->TotalConcavityArea(part));
			this->total_int_profile_area += (quantity * this->TotalIntProfileArea(part));
			nestpart = cluster->get_next_part(&cState);
		}
	}

	return (error);
}

double MtcPartCategoryInfo::TotalConcavityArea(PART_LIST_PART *_part)
{
	double
		total_area(0.0);

	if (_part)
	{
		MtcProfileInfo
			profile_info;
		double
			area,
			dummy;

		if (_part->get_ext_profile() &&
			_part->get_ext_profile()->GetProfileInfo(&profile_info) == 0)
		{
			for (int i=0; i<profile_info.NumberOfConcavities(); i++)
			{
				profile_info.GetConcavityInfo(i, &area, &dummy, &dummy);
				total_area += area;
			}
		}
	}

	return (total_area);
}

double MtcPartCategoryInfo::TotalIntProfileArea(PART_LIST_PART *_part)
{
	double
		total_area(0.0);

	if (_part)
	{
		MTC_PROFILE
			*profile;
		MtcPLPState
			pState;

		profile = _part->FirstIntProfile(&pState);
		while (profile)
		{
			total_area += profile->Area();
			profile = _part->NextIntProfile(&pState);
		}
	}

	return (total_area);
}

int MtcPartCategoryInfo::Calculate(int _total_part_set_quantity, double _total_part_set_area)
{
	int
		error(-1);

	if (_total_part_set_quantity && _total_part_set_area > ACCY && 
		this->total_region_area > ACCY && this->total_part_area > ACCY)
	{
		this->avg_ext_profile_ratio = this->total_ext_profile_area / this->total_region_area;
		this->avg_part_area_ratio = this->total_part_area / this->total_region_area;
		this->area_ratio = this->total_part_area / _total_part_set_area;
		this->fill_area_ratio1 = this->total_fill_area1 / this->total_part_area;
		if (this->total_int_profile_area + this->total_concavity_area > ACCY)
			this->fill_area_ratio2 = this->total_fill_area2 / 
				(this->total_int_profile_area + this->total_concavity_area);
		this->quantity_ratio = (double)this->total_quantity / (double)_total_part_set_quantity;
		error = 0;
	}

	return (error);
}

double MtcPartCategoryInfo::TotalFillArea1()
{
	return (this->total_fill_area1);
}

double MtcPartCategoryInfo::TotalFillArea2()
{
	return (this->total_fill_area2);
}

void MtcPartCategoryInfo::TotalFillArea1(double _total_fill_area1)
{
	this->total_fill_area1 = _total_fill_area1;
}

void MtcPartCategoryInfo::TotalFillArea2(double _total_fill_area2)
{
	this->total_fill_area2 = _total_fill_area2;
}

int MtcPartCategoryInfo::TotalQuantity()
{
	return (this->total_quantity);
}

double MtcPartCategoryInfo::TotalRegionArea()
{
	return (this->total_region_area);
}

double MtcPartCategoryInfo::TotalPartArea()
{
	return (this->total_part_area);
}

int MtcPartCategoryInfo::TotalParts()
{
	return (this->total_parts);
}

MtcPartSetInfo::MtcPartSetInfo()
{
	this->max_region_dimension_ratio = 0.0;
	this->max_ext_profile_area_ratio = 0.0;
}

MtcPartSetInfo::MtcPartSetInfo(MtcPartSetInfo &_info)
{
	*this = _info;
}

MtcPartSetInfo::~MtcPartSetInfo()
{
}

MtcPartSetInfo &MtcPartSetInfo::operator=(MtcPartSetInfo &_info)
{
	if (this != &_info)
	{
		for (int i = MPC_VERY_SMALL; i <= MPC_VERY_LARGE; i++)
			this->categories[i] = _info.categories[i];
		this->max_region_dimension_ratio = _info.max_region_dimension_ratio;
		this->max_ext_profile_area_ratio = _info.max_ext_profile_area_ratio;
	}

	return (*this);
}

int MtcPartSetInfo::GetCategoryInfo(MtcPartCategoryInfo *_info, MTC_PART_CATEGORY _size)
{
	int
		error(-1);

	if (_info && _size >= MPC_VERY_SMALL && _size <= MPC_VERY_LARGE)
	{
		*_info = this->categories[_size];
		error = 0;
	}

	return (error);
}

double MtcPartSetInfo::MaxRegionDimensionRatio()
{
	return (this->max_region_dimension_ratio);
}

double MtcPartSetInfo::MaxExtProfileAreaRatio()
{
	return (this->max_ext_profile_area_ratio);
}

int MtcPartSetInfo::SetCategoryInfo(MtcPartCategoryInfo *_info, MTC_PART_CATEGORY _size)
{
	int
		error(-1);

	if (_info && _size >= MPC_VERY_SMALL && _size <= MPC_VERY_LARGE)
	{
		this->categories[_size] = *_info;
		error = 0;
	}

	return (error);
}

void MtcPartSetInfo::MaxRegionDimensionRatio(double _max_region_dimension_ratio)
{
	this->max_region_dimension_ratio = _max_region_dimension_ratio;
}

void MtcPartSetInfo::MaxExtProfileAreaRatio(double _max_ext_profile_area_ratio)
{
	this->max_ext_profile_area_ratio = _max_ext_profile_area_ratio;
}

void MtcPartSetInfo::Clear()
{
	for (int i = MPC_VERY_SMALL; i <= MPC_VERY_LARGE; i++)
	{
		this->categories[i].Clear();
		this->categories[i].Size((MTC_PART_CATEGORY)i);
	}
}

int MtcPartSetInfo::AddPart(MTC_PART_CATEGORY _size, MtcClusterScore *_cscore)
{
	int
		error(-1);

	if (_cscore && _size >= MPC_VERY_SMALL && _size <= MPC_VERY_LARGE)
	{
		this->categories[_size].AddPart(_cscore);
		error = 0;
	}

	return (error);
}

int MtcPartSetInfo::Calculate(int _total_part_set_quantity, double _total_part_set_area)
{
	int
		error(0);

	for (int i = MPC_VERY_SMALL; i <= MPC_VERY_LARGE; i++)
		this->categories[i].Calculate(_total_part_set_quantity, _total_part_set_area);

	return (error);
}

MtcPartSetAnalyzer::MtcPartSetAnalyzer()
{
}

MtcPartSetAnalyzer::MtcPartSetAnalyzer(MtcPartSetAnalyzer &_analyzer)
{
	*this = _analyzer;
}

MtcPartSetAnalyzer::~MtcPartSetAnalyzer()
{
}

MtcPartSetAnalyzer &MtcPartSetAnalyzer::operator=(MtcPartSetAnalyzer &_analyzer)
{
	if (this != &_analyzer)
	{
		this->unnested_part_set = _analyzer.unnested_part_set;
	}

	return (*this);
}

int MtcPartSetAnalyzer::Set(MtcNestRegionMap *_nrmap, MTC_BOOLEAN _include_priority0_parts)
{
	int
		error(-1);

	if (_nrmap)
	{
		MtcNestRegionMapState
			nrmState;
		MtcClusterScore
			*cscore;
		MTC_PART_CATEGORY
			size;
		MtcPartCategoryInfo
			category;
		int
			total_part_set_quantity(0);
		double
			total_part_set_area(0.0);
		map<int, int>
			shape_type_quantities[MAX_PART_CATEGORIES];
		map<int, int>::iterator
			stq_iter;
		MtcPartCategoryInfo
			pinfo;

		error = 0;
		this->unnested_part_set.Clear();
		cscore = _nrmap->FirstUnnestedClusterScore(&nrmState);
		while (cscore)
		{
			if (_include_priority0_parts || cscore->Cluster()->get_priority() > 0)
			{
				size = this->Size(cscore);
				this->unnested_part_set.AddPart(size, cscore); 
				this->SetFillAreas(_nrmap, size, cscore);
				total_part_set_quantity += cscore->EstimatedFootprintQuantity();
				total_part_set_area += (cscore->EstimatedFootprintQuantity() * cscore->Cluster()->get_area());

				// collect primary shape type info
				stq_iter = shape_type_quantities[size].find((int)cscore->ShapeType());
				if (stq_iter != shape_type_quantities[size].end())
					stq_iter->second += cscore->EstimatedFootprintQuantity();
				else
					shape_type_quantities[size].insert(std::pair<int, int>((int)cscore->ShapeType(), cscore->EstimatedFootprintQuantity()));
			}
			cscore = _nrmap->NextUnnestedClusterScore(&nrmState);
		}


		vector<pair<int, int>> v;
		vector<pair<int, int>>::iterator viter;

		this->unnested_part_set.Calculate(total_part_set_quantity, total_part_set_area);
		for (int i = MPC_VERY_SMALL; i <= MPC_VERY_LARGE; i++)
		{
			if (!shape_type_quantities[size].empty())
			{
				v.assign(shape_type_quantities[i].begin(), shape_type_quantities[i].end());
				std::sort(v.begin(), v.end(), 
					[] (std::pair<int, int> _e1, std::pair<int, int> _e2) {	return (_e1.second > _e2.second); });

				viter = v.begin();
				if (viter != v.end())
				{
					this->unnested_part_set.GetCategoryInfo(&pinfo, (MTC_PART_CATEGORY)i);
					if (viter->first > pinfo.TotalQuantity() / 2)
					{
						pinfo.PrimaryShape((MTC_NEST_SHAPE_TYPE)viter->first);
						this->unnested_part_set.SetCategoryInfo(&pinfo, (MTC_PART_CATEGORY)i);
					}
				}
			}
		}
	}

	return (error);
}

int MtcPartSetAnalyzer::SetFillAreas(MtcNestRegionMap *_nrmap, MTC_PART_CATEGORY _size, MtcClusterScore *_cscore)
{
	int
		error(-1);

	if (_nrmap && _cscore)
	{
		MtcNestRegionMapState
			nrmState;
		MtcClusterScore
			*cscore;
		MTC_PROFILE
			*profile1,
			*profile2;
		int
			quantity1,
			quantity2;
		NEST_PART
			*nestpart1,
			*nestpart2;
		MtcClusterState
			cState1,
			cState2;
		MtcPLPState
			pState1,
			pState2;
		MtcProfileInfo
			profinfo;
		const double
			MIN_PROFILE_AREA = 1.0;

		quantity1 = _cscore->EstimatedFootprintQuantity();
		nestpart1 = _cscore->Cluster()->get_first_part(&cState1);
		while (nestpart1)
		{
			profile1 = nestpart1->get_part()->FirstProfile(&pState1);
			while (profile1)
			{
				if (profile1->Area() > MIN_PROFILE_AREA)
				{
					cscore = _nrmap->FirstUnnestedClusterScore(&nrmState);
					while (cscore)
					{
						if (cscore && cscore->Cluster()->get_priority() > 0)
						{
							quantity2 = cscore->EstimatedFootprintQuantity();
							nestpart2 = cscore->Cluster()->get_first_part(&cState2);
							while (nestpart2)
							{
								profile2 = nestpart2->get_part()->FirstProfile(&pState2);
								while (profile2)
								{
									if (profile2->Area() > MIN_PROFILE_AREA)
										this->UpdateFillAreas(_size, quantity1, quantity2, profile1, profile2);
									profile2 = nestpart2->get_part()->NextProfile(&pState2);
								}
								nestpart2 = cscore->Cluster()->get_next_part(&cState2);
							}
						}
						cscore = _nrmap->NextUnnestedClusterScore(&nrmState);
					}
				}
				profile1 = nestpart1->get_part()->NextProfile(&pState1);
			}
			nestpart1 = _cscore->Cluster()->get_next_part(&cState1);
		}
	}

	return (error);
}

int MtcPartSetAnalyzer::UpdateFillAreas(MTC_PART_CATEGORY _size, int _quantity1, int _quantity2, MTC_PROFILE *_profile1, MTC_PROFILE *_profile2)
{
	int
		error(-1);
	MtcPartCategoryInfo
		category;

	if (_profile1 && _profile2 && _profile1->Area() > 1.0 && _profile2->Area() > 1.0 &&
		this->unnested_part_set.GetCategoryInfo(&category, _size) == 0)
	{
		MtcProfileInfo
			profinfo1,
			profinfo2;
		MTC_PROFILE_INFO	
			info1,
			info2;

		if (_profile1->GetProfileInfo(&profinfo1) == 0 && _profile2->GetProfileInfo(&profinfo2) == 0 &&
			profinfo1.GetProfileInfo(&info1) == 0 && profinfo2.GetProfileInfo(&info2) == 0)
		{
			double
				fill_area1(0.0),
				fill_area2(0.0);
			int
				adj_quantity;

			if (_profile1->Exterior() && _profile2->Exterior())
			{
				int
					idx;

				// does profile1 fit inside profile2s concavities?
				for (idx = 0; idx < profinfo2.NumberOfConcavities() && 
					profinfo2.GetConcavityInfo(idx, &info2) == 0; idx++)
				{
					if (info1.max_dist_from_boundary < info2.max_dist_from_boundary &&
						info2.max_contained_distance > info1.max_contained_distance * 0.5)
						fill_area1 = _profile1->Area() * min(_quantity1, _quantity2) * 
							min(1.0, info2.max_contained_distance / info1.max_contained_distance);

				}

				// does profile2 fit inside profile1s concavities?
				profinfo2.GetProfileInfo(&info2);
				for (idx = 0; idx < profinfo1.NumberOfConcavities() && 
					profinfo1.GetConcavityInfo(idx, &info1) == 0; idx++)
				{
					if (info2.max_dist_from_boundary < info1.max_dist_from_boundary &&
						info1.max_contained_distance > info2.max_contained_distance * 0.5)
					{
						fill_area2 = info1.area * min(_quantity1, _quantity2) * 
							min(1.0, info2.max_contained_distance / info1.max_contained_distance);
					}
				}
			}
			else if (_profile1->Exterior() && _profile2->Interior() &&
				info1.max_contained_distance < info2.max_contained_distance &&
				info1.max_dist_from_boundary < info2.max_dist_from_boundary)
			{
				// profile1 fits inside of profile2
				if (_quantity1 < _quantity2)
					adj_quantity = _quantity1;
				else
				{
					adj_quantity = max(1, (int)(info2.max_contained_distance / 
						info1.max_contained_distance));
					adj_quantity *= max(1, (int)(info2.max_dist_from_boundary / 
						info1.max_dist_from_boundary));
					adj_quantity *= _quantity2;
					if (adj_quantity > _quantity1)
						adj_quantity = _quantity1;
				}

				fill_area1 = _profile1->Area() * adj_quantity;
			}
			else if (_profile1->Interior() && _profile2->Exterior() &&
				info2.max_contained_distance < info1.max_contained_distance &&
				info2.max_dist_from_boundary < info1.max_dist_from_boundary)
			{
				// profile2 fits inside of profile1
				fill_area2 = _profile1->Area() * min(_quantity1, _quantity2);
			}

			category.TotalFillArea1(category.TotalFillArea1() + fill_area1);
			category.TotalFillArea2(category.TotalFillArea2() + fill_area2);
			this->unnested_part_set.SetCategoryInfo(&category, _size);
			error = 0;
		}
	}

	return (error);
}

double MtcPartSetAnalyzer::EstimatedUtilization()
{
	double
		utilization(0.0);

	return (utilization);
}

MTC_PART_CATEGORY MtcPartSetAnalyzer::Size(MtcClusterScore *_cscore)
{
	MTC_PART_CATEGORY
		size(MPC_UNKNOWN);

	if (_cscore)
	{
		if (_cscore->RegionRatio() < 0.01)
			size = MPC_VERY_SMALL;	// > 100 parts per region
		else if (_cscore->RegionRatio() < 0.05)
			size = MPC_SMALL;		// 20-100 parts per region
		else if (_cscore->RegionRatio() < 0.1)
			size = MPC_MEDIUM;		// 10-20 parts per region
		else if (_cscore->RegionRatio() < 0.5)
			size = MPC_LARGE;		// 2-10 parts per region
		else
			size = MPC_VERY_LARGE;	// 1 part per plate 
	}

	return (size);
}

MtcPartSetInfo *MtcPartSetAnalyzer::GetPartSetInfo(MtcPartSetInfo *_buf)
{
	MtcPartSetInfo
		*info(0);

	if (_buf)
	{
		*_buf = this->unnested_part_set;
		info = _buf;
	}

	return (info);
}

#if defined(MTC_CLASS_TEST_)
#include "spacemap.hpp"

MtcPartSetAnalyzerTest::MtcPartSetAnalyzerTest(wostream *_stream)
{
	this->stream = _stream;
}

MtcPartSetAnalyzerTest::~MtcPartSetAnalyzerTest()
{
}

void MtcPartSetAnalyzerTest::Test(JOB *_job)
{
	this->SetProfileInfo(_job);
	this->EstimatedUtilizationTest(_job);
	this->MiscTest(_job);
}

void MtcPartSetAnalyzerTest::EstimatedUtilizationTest(JOB *_job)
{
	//
	// set up
	//
	*stream << _T("\nStart of MtcPartSetAnalyzerTest::EstimatedUtilizationTest().") << endl << endl;

	//
	// do something
	//
	MtcPartSetAnalyzer
		analyzer;

	//
	// clean up
	//
	*stream << _T("\nEnd of MtcPartSetAnalyzerTest::EstimatedUtilizationTest().") << endl << endl;
}

void MtcPartSetAnalyzerTest::MiscTest(JOB *_job)
{
	//
	// set up
	//
	*stream << _T("\nStart of MtcPartSetAnalyzerTest::MiscTest().") << endl << endl;

	//
	// do something
	//
	MtcPartSetAnalyzer
		analyzer;
	MtcNestRegionMap
		nrmap(_job, _job->ActiveNest(), _job->ActiveNest()->get_sheet().get_region());

	nrmap.SetUnnestedClusters();
	analyzer.Set(&nrmap);
	this->Output(&analyzer);

	//
	// clean up
	//
	*stream << _T("\nEnd of MtcPartSetAnalyzerTest::MiscTest().") << endl << endl;
}

void MtcPartSetAnalyzerTest::Output(MtcPartSetAnalyzer *_analyzer)
{
	if (_analyzer)
	{
		MtcPartCategoryInfo
			info;
		TCHAR
			buffer[MAXPATH];

		*stream << _T("Analyzer [") << _analyzer << _T("]:") << endl;
		for (int size = MPC_VERY_SMALL; size <= MPC_VERY_LARGE; size++)
		{
			_analyzer->unnested_part_set.GetCategoryInfo(&info, (MTC_PART_CATEGORY)size);
			*stream << _T("  Category: ") << this->Category(info.Size(), buffer) <<  endl;
			*stream << _T("    AvgExtProfileRatio: ") << info.AvgExtProfileRatio() <<  endl;
			*stream << _T("    AvgPartAreaRatio: ") << info.AvgPartAreaRatio() <<  endl;
			*stream << _T("    AreaRatio: ") << info.AreaRatio() <<  endl;
			*stream << _T("    FillAreaRatio1: ") << info.FillAreaRatio1() <<  endl;
			*stream << _T("    FillAreaRatio2: ") << info.FillAreaRatio2() <<  endl;
			*stream << _T("    QuantityRatio: ") << info.QuantityRatio() <<  endl;
			*stream << _T("    TotalFillArea1: ") << info.TotalFillArea1() <<  endl;
			*stream << _T("    TotalFillArea2: ") << info.TotalFillArea2() <<  endl;
		}
	}
}

TCHAR *MtcPartSetAnalyzerTest::Category(MTC_PART_CATEGORY _size, TCHAR *_buffer)
{
	switch (_size)
	{
		case MPC_VERY_SMALL:
			_tcscpy(_buffer, _T("MPC_VERY_SMALL"));
			break;

		case MPC_SMALL:
			_tcscpy(_buffer, _T("MPC_SMALL"));
			break;

		case MPC_MEDIUM:
			_tcscpy(_buffer, _T("MPC_MEDIUM"));
			break;

		case MPC_LARGE:
			_tcscpy(_buffer, _T("MPC_LARGE"));
			break;

		case MPC_VERY_LARGE:
			_tcscpy(_buffer, _T("MPC_VERY_LARGE"));
			break;

		case MPC_UNKNOWN:
			_tcscpy(_buffer, _T("MPC_UNKNOWN"));
			break;

		default:
			_tcscpy(_buffer, _T("(no idea!)"));
			break;
	}

	return (_buffer);
}

void MtcPartSetAnalyzerTest::SetProfileInfo(JOB *_job)
{
	if (_job)
	{
		PART_LIST_PART
			*part;
		MtcPartManagerState
			pState;
		MtcSpaceMap
			smap;

		part = _job->get_part_manager().FirstPart(-1, &pState, 1);
		while (part)
		{
			smap.SetProfileDistances(part);
			part = _job->get_part_manager().NextPart(-1, &pState, 1);
		}
	}
}

#endif