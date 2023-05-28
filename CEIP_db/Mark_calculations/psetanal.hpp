#ifndef PART_SET_ANALYZER_HPP
#define PART_SET_ANALYZER_HPP

#include "nrmap.hpp"

#define MAX_PART_CATEGORIES 5

typedef enum { MPC_VERY_SMALL, MPC_SMALL, MPC_MEDIUM, MPC_LARGE, MPC_VERY_LARGE, MPC_UNKNOWN }
	MTC_PART_CATEGORY;

class MtcPartSetAnalyzerTest;

class MtcPartCategoryInfo
{
public:
	MtcPartCategoryInfo();
	MtcPartCategoryInfo(MtcPartCategoryInfo &_info);
	~MtcPartCategoryInfo();
	MtcPartCategoryInfo &operator=(MtcPartCategoryInfo &_info);

	MTC_PART_CATEGORY Size();
	double AvgExtProfileRatio();
	double AvgPartAreaRatio();
	double AreaRatio();
	double FillAreaRatio1();
	double FillAreaRatio2();
	double QuantityRatio();
	double TotalFillArea1();
	double TotalFillArea2();
	MTC_NEST_SHAPE_TYPE PrimaryShape();

	void Size(MTC_PART_CATEGORY _size);
	void AvgExtProfileRatio(double _avg_ext_profile_ratio);
	void AvgPartAreaRatio(double _avg_part_area_ratio);
	void AreaRatio(double _area_ratio);
	void FillAreaRatio1(double _fill_area_ratio1);
	void FillAreaRatio2(double _fill_area_ratio2);
	void QuantityRatio(double _quantity_ratio);
	void TotalFillArea1(double _total_fill_area1);
	void TotalFillArea2(double _total_fill_area2);
	void PrimaryShape(MTC_NEST_SHAPE_TYPE _primary_shape);

	int TotalQuantity();
	double TotalRegionArea();
	double TotalPartArea();
	int TotalParts();

	void Clear();
	int AddPart(MtcClusterScore *_cscore);
	int Calculate(int _total_part_set_quantity, double _total_part_set_area);

private:
	MTC_PART_CATEGORY
		size;
	double
		avg_ext_profile_ratio,
		avg_part_area_ratio,
		area_ratio,
		fill_area_ratio1,
		fill_area_ratio2,
		quantity_ratio;
	// used to calculate the above ratios
	// all values consider only the quantity of parts that can fit in the region
	double
		total_region_area,			// total region area of all parts in category
		total_part_area,			// total area of all parts in category
		total_ext_profile_area,		// total exterior profile area of all parts in category
		total_concavity_area,		// total concavity area of all parts in category
		total_int_profile_area,		// total interior profile area of all parts in category
		total_fill_area1,			// total area of all exteriors that can fit inside any other part in part set
		total_fill_area2;			// total area of all interiors/concavities usable for filling in category
	int
		total_quantity,				// total quantity of all parts in the category
		total_parts;				// total number of parts in the category
	MTC_NEST_SHAPE_TYPE
		primary_shape;

	double TotalConcavityArea(PART_LIST_PART *_part);
	double TotalIntProfileArea(PART_LIST_PART *_part);
};

class MtcPartSetInfo
{
public:
	MtcPartSetInfo();
	MtcPartSetInfo(MtcPartSetInfo &_info);
	~MtcPartSetInfo();
	MtcPartSetInfo &operator=(MtcPartSetInfo &_info);

	int GetCategoryInfo(MtcPartCategoryInfo *_info, MTC_PART_CATEGORY _size);
	double MaxRegionDimensionRatio();
	double MaxExtProfileAreaRatio();

	int SetCategoryInfo(MtcPartCategoryInfo *_info, MTC_PART_CATEGORY _size);
	void MaxRegionDimensionRatio(double _max_region_dimension_ratio);
	void MaxExtProfileAreaRatio(double _max_ext_profile_area_ratio);

	void Clear();
	int AddPart(MTC_PART_CATEGORY _size, MtcClusterScore *_cscore);
	int Calculate(int _total_part_set_quantity, double _total_part_set_area);

private:
	MtcPartCategoryInfo
		categories[MAX_PART_CATEGORIES];
	double
		max_region_dimension_ratio,
		max_ext_profile_area_ratio;
};

class MtcPartSetAnalyzer
{
	friend class MtcPartSetAnalyzerTest;

public:
	MtcPartSetAnalyzer();
	MtcPartSetAnalyzer(MtcPartSetAnalyzer &_analyzer);
	~MtcPartSetAnalyzer();
	MtcPartSetAnalyzer &operator=(MtcPartSetAnalyzer &_analyzer);

	int Set(MtcNestRegionMap *_nrmap, MTC_BOOLEAN _include_priority0_parts = TRUE);
	double EstimatedUtilization();
	MtcPartSetInfo *GetPartSetInfo(MtcPartSetInfo *_buf);

private:
	MtcPartSetInfo
		unnested_part_set;

	MTC_PART_CATEGORY Size(MtcClusterScore *_cscore);
	int SetFillAreas(MtcNestRegionMap *_nrmap, MTC_PART_CATEGORY _size, MtcClusterScore *_cscore);
	int UpdateFillAreas(MTC_PART_CATEGORY _size, int _quantity1, int _quantity2, MTC_PROFILE *_profile1, MTC_PROFILE *_profile2);
};

#if defined(MTC_CLASS_TEST_)

class MtcPartSetAnalyzerTest
{
public:
	MtcPartSetAnalyzerTest(wostream *_stream);
	~MtcPartSetAnalyzerTest();
	void Test(JOB *_job);
	void EstimatedUtilizationTest(JOB *_job);
	void MiscTest(JOB *_job);

private:
	wostream
		*stream;

	void Output(MtcPartSetAnalyzer *_analyzer);
	TCHAR *Category(MTC_PART_CATEGORY _size, TCHAR *_buffer);
	void SetProfileInfo(JOB *_job);
};

#endif
#endif
