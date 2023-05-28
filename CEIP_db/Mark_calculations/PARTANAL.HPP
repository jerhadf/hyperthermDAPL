#ifndef PART_ANALYZER_HPP
#define PART_ANALYZER_HPP

#include "clsscore.hpp"
#include "cluster.hpp"

#define BASIC_PATTERN			0
#define INTERMEDIATE_PATTERN	1
#define ADVANCED_PATTERN		2
#define LARGE_ANGLE_STEP		5.0
#define MEDIUM_ANGLE_STEP		1.0
#define SMALL_ANGLE_STEP		0.5
#define SMALLEST_ANGLE_STEP	0.1

#define LARGE_PART_THRESHOLD				30
#define RECTANGULARISHNESS_THRESHOLD	85
#define FULL_ARRAY_SCORE 0.50
#define MATERIAL_USED_FACTOR	60
#define MATERIAL_MISSING_FACTOR	65
#define CONCAVITY_FACTOR		30
#define EDGINESS_PERIMETER_FACTOR		0.01
#define CORNERINESS_AREA_FACTOR	0.80

// 1.15 degrees = 0.02 radians
#define RIGHT_ANGLE_THRESHOLD 1.15
#define LARGEST_PRATIO_THRESHOLD 0.5
#define LARGE_PRATIO_THRESHOLD 0.2
#define MEDIUM_PRATIO_THRESHOLD 0.05
#define SMALL_PRATIO_THRESHOLD 0.01

#ifdef OLD_SCORES
	#define LARGEST_PRATIO_SCORE 90
	#define LARGE_PRATIO_SCORE 85
	#define MEDIUM_PRATIO_SCORE 80
	#define SMALL_PRATIO_SCORE 75
	#define SMALLEST_PRATIO_SCORE 60
	#define LARGEST_PRATIO_BASE_SCORE 75
	#define LARGE_PRATIO_BASE_SCORE 50
	#define MEDIUM_PRATIO_BASE_SCORE 35
	#define SMALL_PRATIO_BASE_SCORE 25
	#define SMALLEST_PRATIO_BASE_SCORE 20
#else
	#define LARGEST_PRATIO_SCORE 70
	#define LARGE_PRATIO_SCORE 50
	#define MEDIUM_PRATIO_SCORE 40
	#define SMALL_PRATIO_SCORE 30
	#define SMALLEST_PRATIO_SCORE 20
	#define LARGEST_PRATIO_BASE_SCORE 80
	#define LARGE_PRATIO_BASE_SCORE 75
	#define MEDIUM_PRATIO_BASE_SCORE 70
	#define SMALL_PRATIO_BASE_SCORE 60
	#define SMALLEST_PRATIO_BASE_SCORE 50
#endif

#define SAGITTA_RATIO 0.0007
#define EDGY_RADIUS_FACTOR	0.005

class JOB;

class MtcAnalyzerWeights
{
	public:
		MtcAnalyzerWeights(double _large_part_threshold = LARGE_PART_THRESHOLD, double _rectangularishness_threshold = RECTANGULARISHNESS_THRESHOLD,
			double _full_array_score = FULL_ARRAY_SCORE, double _material_used_factor = MATERIAL_USED_FACTOR, double _material_missing_factor = MATERIAL_MISSING_FACTOR,
			double _concavity_factor = CONCAVITY_FACTOR, double _edginess_perimeter_factor = EDGINESS_PERIMETER_FACTOR,
			double _corneriness_area_factor = CORNERINESS_AREA_FACTOR, double _right_angle_threshold = RIGHT_ANGLE_THRESHOLD,
			double _largest_pratio_threshold = LARGEST_PRATIO_THRESHOLD, double _large_pratio_threshold = LARGE_PRATIO_THRESHOLD,
			double _medium_pratio_threshold = MEDIUM_PRATIO_THRESHOLD, double _small_pratio_threshold = SMALL_PRATIO_THRESHOLD,
			double _largest_pratio_score = LARGEST_PRATIO_SCORE, double _large_pratio_score = LARGE_PRATIO_SCORE,
			double _medium_pratio_score = MEDIUM_PRATIO_SCORE, double _small_pratio_score = SMALL_PRATIO_SCORE,
			double _smallest_pratio_score = SMALLEST_PRATIO_SCORE, double _largest_pratio_base_score = LARGEST_PRATIO_BASE_SCORE,
			double _large_pratio_base_score = LARGE_PRATIO_BASE_SCORE, double _medium_pratio_base_score = MEDIUM_PRATIO_BASE_SCORE,
			double _small_pratio_base_score = SMALL_PRATIO_BASE_SCORE, double _smallest_pratio_base_score = SMALLEST_PRATIO_BASE_SCORE,
			double _sagitta_ratio = SAGITTA_RATIO, double _edgy_radius_factor = EDGY_RADIUS_FACTOR);
		MtcAnalyzerWeights(MtcAnalyzerWeights &_weights);
		~MtcAnalyzerWeights();
		MtcAnalyzerWeights &operator=(MtcAnalyzerWeights &_weights);

		friend wostream &operator<<(wostream &_stream, MtcAnalyzerWeights &_weights);

		void Set(double _large_part_threshold, double _rectangularishness_threshold, double _full_array_score,
			double _material_used_factor, double _material_missing_factor, double _concavity_factor, double _edginess_perimeter_factor,
			double _corneriness_area_factor, double _right_angle_threshold, double _largest_pratio_threshold,
			double _large_pratio_threshold, double _medium_pratio_threshold, double _small_pratio_threshold,
			double _largest_pratio_score, double _large_pratio_score, double _medium_pratio_score,
			double _small_pratio_score, double _smallest_pratio_score, double _largest_pratio_base_score,
			double _large_pratio_base_score, double _medium_pratio_base_score, double _small_pratio_base_score,
			double _smallest_pratio_base_score, double _sagitta_ratio, double _edgy_radius_factor);

		void LargePartThreshold(double _large_part_threshold) { large_part_threshold = _large_part_threshold; }
		double LargePartThreshold() { return (large_part_threshold); }

		void RectangularishnessThreshold(double _rectangularishness_threshold) { rectangularishness_threshold = _rectangularishness_threshold; }
		double RectangularishnessThreshold() { return (rectangularishness_threshold); }

		void FullArrayScore(double _full_array_score) { full_array_score = _full_array_score; }
		double FullArrayScore() { return (full_array_score); }

		void MaterialUsedFactor(double _material_used_factor) { material_used_factor = _material_used_factor; }
		double MaterialUsedFactor() { return (material_used_factor); }

		void MaterialMissingFactor(double _material_missing_factor) { material_missing_factor = _material_missing_factor; }
		double MaterialMissingFactor() { return (material_missing_factor); }

		void ConcavityFactor(double _concavity_factor) { concavity_factor = _concavity_factor; }
		double ConcavityFactor() { return (concavity_factor); }

		void EdginessPerimeterFactor(double _edginess_perimeter_factor) { edginess_perimeter_factor = _edginess_perimeter_factor; }
		double EdginessPerimeterFactor() { return (edginess_perimeter_factor); }

		void CornerinessAreaFactor(double _corneriness_area_factor) { corneriness_area_factor = _corneriness_area_factor; }
		double CornerinessAreaFactor() { return (corneriness_area_factor); }

		void RightAngleThreshold(double _right_angle_threshold) { right_angle_threshold = _right_angle_threshold; }
		double RightAngleThreshold() { return (right_angle_threshold); }

		void LargestPratioThreshold(double _largest_pratio_threshold) { largest_pratio_threshold = _largest_pratio_threshold; }
		double LargestPratioThreshold() { return (largest_pratio_threshold); }

		void LargePratioThreshold(double _large_pratio_threshold) { large_pratio_threshold = _large_pratio_threshold; }
		double LargePratioThreshold() { return (large_pratio_threshold); }

		void MediumPratioThreshold(double _medium_pratio_threshold) { medium_pratio_threshold = _medium_pratio_threshold; }
		double MediumPratioThreshold() { return (medium_pratio_threshold); }

		void SmallPratioThreshold(double _small_pratio_threshold) { small_pratio_threshold = _small_pratio_threshold; }
		double SmallPratioThreshold() { return (small_pratio_threshold); }

		void LargestPratioScore(double _largest_pratio_score) { largest_pratio_score = _largest_pratio_score; }
		double LargestPratioScore() { return (largest_pratio_score); }

		void LargePratioScore(double _large_pratio_score) { large_pratio_score = _large_pratio_score; }
		double LargePratioScore() { return (large_pratio_score); }

		void MediumPratioScore(double _medium_pratio_score) { medium_pratio_score = _medium_pratio_score; }
		double MediumPratioScore() { return (medium_pratio_score); }

		void SmallPratioScore(double _small_pratio_score) { small_pratio_score = _small_pratio_score; }
		double SmallPratioScore() { return (small_pratio_score); }

		void SmallestPratioScore(double _smallest_pratio_score) { smallest_pratio_score = _smallest_pratio_score; }
		double SmallestPratioScore() { return (smallest_pratio_score); }

		void LargestPratioBaseScore(double _largest_pratio_base_score) { largest_pratio_base_score = _largest_pratio_base_score; }
		double LargestPratioBaseScore() { return (largest_pratio_base_score); }

		void LargePratioBaseScore(double _large_pratio_base_score) { large_pratio_base_score = _large_pratio_base_score; }
		double LargePratioBaseScore() { return (large_pratio_base_score); }

		void MediumPratioBaseScore(double _medium_pratio_base_score) { medium_pratio_base_score = _medium_pratio_base_score; }
		double MediumPratioBaseScore() { return (medium_pratio_base_score); }

		void SmallPratioBaseScore(double _small_pratio_base_score) { small_pratio_base_score = _small_pratio_base_score; }
		double SmallPratioBaseScore() { return (small_pratio_base_score); }

		void SmallestPratioBaseScore(double _smallest_pratio_base_score) { smallest_pratio_base_score = _smallest_pratio_base_score; }
		double SmallestPratioBaseScore() { return (smallest_pratio_base_score); }

		void SagittaRatio(double _sagitta_ratio) { sagitta_ratio = _sagitta_ratio; }
		double SagittaRatio() { return (sagitta_ratio); }

		void EdgyRadiusFactor(double _edgy_radius_factor) { edgy_radius_factor = _edgy_radius_factor; }
		double EdgyRadiusFactor() { return (edgy_radius_factor); }

	private:
		double
			large_part_threshold,
			rectangularishness_threshold,
			full_array_score,
			material_used_factor,
			material_missing_factor,
			concavity_factor,
			edginess_perimeter_factor,
			corneriness_area_factor,
			right_angle_threshold,
			largest_pratio_threshold,
			large_pratio_threshold,
			medium_pratio_threshold,
			small_pratio_threshold,
			largest_pratio_score,
			large_pratio_score,
			medium_pratio_score,
			small_pratio_score,
			smallest_pratio_score,
			largest_pratio_base_score,
			large_pratio_base_score,
			medium_pratio_base_score,
			small_pratio_base_score,
			smallest_pratio_base_score,
			edgy_radius_factor,
			sagitta_ratio;
};

class MtcPartAnalyzer
{
	public:
		MtcPartAnalyzer();
		MtcPartAnalyzer(MtcPartAnalyzer &_part_analyzer);
		~MtcPartAnalyzer();
		MtcPartAnalyzer &operator=(MtcPartAnalyzer &_part_analyzer);
		friend wostream &operator<<(wostream &_stream, MtcPartAnalyzer &_analyzer);

		MtcClusterScore *Analyze(CLUSTER *_cluster, REGION _region, REGION _kerfed_region, double _available_area,
			JOB *_job = 0, MtcClusterScore *_score = 0, MtcClusterScore *_similar_part_score = 0);
		MtcClusterScore *ClusterScore();

		void CreateVoids(MtcNestShape *_shape, REGION _region, MtcNestShapeGroup *_void_group);

	private:
		void Clear();
		void Set(CLUSTER *_cluster, REGION _region, REGION _kerfed_region, double _available_area,
			JOB *_job, MtcClusterScore *_score = 0);
		void OneTimeAnalysis(MtcClusterScore *_similar_part_score);
		void AnalyzeBigness();
		void AnalyzeInterlockability();
		void AnalyzeRectangularishness();
		void AnalyzeTotalFootprint();
		void AnalyzeClcArrayability();
		void AnalyzeClcArrayScore();
		void AnalyzePatternArrayability();
		void AnalyzePatternArrayStrategy();
		void AnalyzeFillerRotation();
		void InitialRotatabilityComputation(MtcClusterScore *_similar_part_score);
		void AnalyzeRotatability(MtcClusterScore *_similar_part_score);
		void AnalyzeFilleriness();
		void AnalyzeExteriorArcs();
		void AnalyzeShapeAndTendency();
		MtcNestShape *GetNestShape();

		void AnalyzeCorneriness();
		void AnalyzeRighteousness();
		void AnalyzeSlopiness();
		void AnalyzeEdginess();
		void AnalyzeConcavityOrientation();
		void AnalyzeProfileNestingScore();

		double RegionAngleForLength(double _length, int _checking_height);

		void Cluster(CLUSTER *_cluster) { this->cluster = _cluster; }
		CLUSTER *Cluster() { return (this->cluster); }

		void Region(REGION _region) { this->region = _region; }
		REGION &Region() { return (this->region); }
		void KerfedRegion(REGION _region) { this->kerfed_region = _region; }
		REGION &KerfedRegion() { return (this->kerfed_region); }

		void Job(JOB *_job) { this->job = _job; }
		JOB *Job() { return (this->job); }

		void AvailableArea(double _area) { this->available_area = _area; }
		double AvailableArea() { return (this->available_area); }

		MtcAnalyzerWeights *Weights() { return &(this->weights); }

		void ClusterScore(MtcClusterScore *_cluster_score, int _assign_to_cluster = 1);
		void BuildNestShapeGroup(MtcNestShapeGroup *_ngroup, CLUSTER *_cluster = 0);

		int FixArcsCloseToBoundaries(MtcNestShape *_shape, MtcNestShape *_region_shape, REGION &_region);

		CLUSTER
			*cluster;
		REGION
			region,
			kerfed_region;
		JOB
			*job;
		MtcClusterScore
			*cluster_score;
		MtcNestShapeGroup
			working_nsg,
			original_nshapes;
		MtcAnalyzerWeights
			weights;
		double
			available_area;
};

#if defined(MTC_CLASS_TEST_)

class MtcPartAnalyzerTest
{
public:
	MtcPartAnalyzerTest(CLUSTER_GROUP *_cgrp, JOB *_job, wostream *_stream);
	~MtcPartAnalyzerTest();
	void Test();
	void AnalyzeTest();
private:
	wostream
		*stream;
	CLUSTER_GROUP
		*cgrp;
	JOB
		*job;
};

#endif
#endif
