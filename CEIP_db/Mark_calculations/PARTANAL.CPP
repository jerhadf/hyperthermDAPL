#include "partanal.hpp"
#include "clcgen.hpp"
#include "util.hpp"

#define SLOPINESS_THRESHOLD	60.0
//
// May need to add knowledge of other parts for judging whether concavities are useful
// Keep track of list of concavities?
// Profile nesting score - generally large parts with low quantity
// Redo filler scoring
//	Identify some new shapes we don't currently handle - arc trapezoid, paddle, etc.
//

extern DEFAULTS defaults;

MtcAnalyzerWeights::MtcAnalyzerWeights(double _large_part_threshold, double _rectangularishness_threshold, double _full_array_score,
	double _material_used_factor, double _material_missing_factor, double _concavity_factor, double _edginess_perimeter_factor,
	double _corneriness_area_factor, double _right_angle_threshold, double _largest_pratio_threshold,
	double _large_pratio_threshold, double _medium_pratio_threshold, double _small_pratio_threshold,
	double _largest_pratio_score, double _large_pratio_score, double _medium_pratio_score,
	double _small_pratio_score, double _smallest_pratio_score, double _largest_pratio_base_score,
	double _large_pratio_base_score, double _medium_pratio_base_score, double _small_pratio_base_score,
	double _smallest_pratio_base_score, double _sagitta_ratio, double _edgy_radius_factor)
{
	Set(_large_part_threshold, _rectangularishness_threshold, _full_array_score, _material_used_factor,
		_material_missing_factor, _concavity_factor, _edginess_perimeter_factor, _corneriness_area_factor,
		_right_angle_threshold, _largest_pratio_threshold, _large_pratio_threshold, _medium_pratio_threshold,
		_small_pratio_threshold, _largest_pratio_score, _large_pratio_score, _medium_pratio_score,
		_small_pratio_score, _smallest_pratio_score, _largest_pratio_base_score, _large_pratio_base_score,
		_medium_pratio_base_score, _small_pratio_base_score, _smallest_pratio_base_score, _sagitta_ratio, _edgy_radius_factor);
}

MtcAnalyzerWeights::MtcAnalyzerWeights(MtcAnalyzerWeights &_weights)
{
	*this = _weights;
}

MtcAnalyzerWeights::~MtcAnalyzerWeights()
{
}

MtcAnalyzerWeights &MtcAnalyzerWeights::operator=(MtcAnalyzerWeights &_weights)
{
	LargePartThreshold(_weights.LargePartThreshold());
	RectangularishnessThreshold(_weights.RectangularishnessThreshold());
	FullArrayScore(_weights.FullArrayScore());
	MaterialUsedFactor(_weights.MaterialUsedFactor());
	MaterialMissingFactor(_weights.MaterialMissingFactor());
	ConcavityFactor(_weights.ConcavityFactor());
	EdginessPerimeterFactor(_weights.EdginessPerimeterFactor());
	CornerinessAreaFactor(_weights.CornerinessAreaFactor());
	RightAngleThreshold(_weights.RightAngleThreshold());
	LargestPratioThreshold(_weights.LargestPratioThreshold());
	LargePratioThreshold(_weights.LargePratioThreshold());
	MediumPratioThreshold(_weights.MediumPratioThreshold());
	SmallPratioThreshold(_weights.SmallPratioThreshold());
	LargestPratioScore(_weights.LargestPratioScore());
	LargePratioScore(_weights.LargePratioScore());
	MediumPratioScore(_weights.MediumPratioScore());
	SmallPratioScore(_weights.SmallPratioScore());
	SmallestPratioScore(_weights.SmallestPratioScore());
	LargestPratioBaseScore(_weights.LargestPratioBaseScore());
	LargePratioBaseScore(_weights.LargePratioBaseScore());
	MediumPratioBaseScore(_weights.MediumPratioBaseScore());
	SmallPratioBaseScore(_weights.SmallPratioBaseScore());
	SmallestPratioBaseScore(_weights.SmallestPratioBaseScore());
	SagittaRatio(_weights.SagittaRatio());
	EdgyRadiusFactor(_weights.EdgyRadiusFactor());
	return (*this);
}

wostream &operator<<(wostream &_stream, MtcAnalyzerWeights &_weights)
{
	_stream << _T("analyzer weights [") << &_weights << _T("]:") << endl;

	_stream << _T("LargePartThreshold: ") << _weights.LargePartThreshold() << endl;
	_stream << _T("RectangularishnessThreshold: ") << _weights.RectangularishnessThreshold() << endl;
	_stream << _T("FullArrayScore: ") << _weights.FullArrayScore() << endl;
	_stream << _T("MaterialUsedFactor: ") << _weights.MaterialUsedFactor() << endl;
	_stream << _T("MaterialMissingFactor: ") << _weights.MaterialMissingFactor() << endl;
	_stream << _T("ConcavityFactor: ") << _weights.ConcavityFactor() << endl;
	_stream << _T("EdginessPerimeterFactor: ") << _weights.EdginessPerimeterFactor() << endl;
	_stream << _T("CornerinessAreaFactor: ") << _weights.CornerinessAreaFactor() << endl;
	_stream << _T("RightAngleThreshold: ") << _weights.RightAngleThreshold() << endl;
	_stream << _T("LargestPratioThreshold: ") << _weights.LargestPratioThreshold() << endl;
	_stream << _T("LargePratioThreshold: ") << _weights.LargePratioThreshold() << endl;
	_stream << _T("MediumPratioThreshold: ") << _weights.MediumPratioThreshold() << endl;
	_stream << _T("SmallPratioThreshold: ") << _weights.SmallPratioThreshold() << endl;
	_stream << _T("LargestPratioScore: ") << _weights.LargestPratioScore() << endl;
	_stream << _T("LargePratioScore: ") << _weights.LargePratioScore() << endl;
	_stream << _T("MediumPratioScore: ") << _weights.MediumPratioScore() << endl;
	_stream << _T("SmallPratioScore: ") << _weights.SmallPratioScore() << endl;
	_stream << _T("SmallestPratioScore: ") << _weights.SmallestPratioScore() << endl;
	_stream << _T("LargestPratioBaseScore: ") << _weights.LargestPratioBaseScore() << endl;
	_stream << _T("LargePratioBaseScore: ") << _weights.LargePratioBaseScore() << endl;
	_stream << _T("MediumPratioBaseScore: ") << _weights.MediumPratioBaseScore() << endl;
	_stream << _T("SmallPratioBaseScore: ") << _weights.SmallPratioBaseScore() << endl;
	_stream << _T("SmallestPratioBaseScore: ") << _weights.SmallestPratioBaseScore() << endl;
	_stream << _T("SagittaRatio: ") << _weights.SagittaRatio() << endl;
	_stream << _T("EdgyRadiusFactor: ") << _weights.EdgyRadiusFactor() << endl;

	_stream << _T("end of analyzer weights [") << &_weights << _T("]") << endl;
	return (_stream);
}

void MtcAnalyzerWeights::Set(double _large_part_threshold, double _rectangularishness_threshold, double _full_array_score,
	double _material_used_factor, double _material_missing_factor, double _concavity_factor, double _edginess_perimeter_factor,
	double _corneriness_area_factor, double _right_angle_threshold, double _largest_pratio_threshold,
	double _large_pratio_threshold, double _medium_pratio_threshold, double _small_pratio_threshold,
	double _largest_pratio_score, double _large_pratio_score, double _medium_pratio_score,
	double _small_pratio_score, double _smallest_pratio_score, double _largest_pratio_base_score,
	double _large_pratio_base_score, double _medium_pratio_base_score, double _small_pratio_base_score,
	double _smallest_pratio_base_score, double _sagitta_ratio, double _edgy_radius_factor)
{
	LargePartThreshold(_large_part_threshold);
	RectangularishnessThreshold(_rectangularishness_threshold);
	FullArrayScore(_full_array_score);
	MaterialUsedFactor(_material_used_factor);
	MaterialMissingFactor(_material_missing_factor);
	ConcavityFactor(_concavity_factor);
	EdginessPerimeterFactor(_edginess_perimeter_factor);
	CornerinessAreaFactor(_corneriness_area_factor);
	RightAngleThreshold(_right_angle_threshold);
	LargestPratioThreshold(_largest_pratio_threshold);
	LargePratioThreshold(_large_pratio_threshold);
	MediumPratioThreshold(_medium_pratio_threshold);
	SmallPratioThreshold(_small_pratio_threshold);
	LargestPratioScore(_largest_pratio_score);
	LargePratioScore(_large_pratio_score);
	MediumPratioScore(_medium_pratio_score);
	SmallPratioScore(_small_pratio_score);
	SmallestPratioScore(_smallest_pratio_score);
	LargestPratioBaseScore(_largest_pratio_base_score);
	LargePratioBaseScore(_large_pratio_base_score);
	MediumPratioBaseScore(_medium_pratio_base_score);
	SmallPratioBaseScore(_small_pratio_base_score);
	SmallestPratioBaseScore(_smallest_pratio_base_score);
	SagittaRatio(_sagitta_ratio);
	EdgyRadiusFactor(_edgy_radius_factor);
}

MtcPartAnalyzer::MtcPartAnalyzer()
{
	Clear();
}

MtcPartAnalyzer::MtcPartAnalyzer(MtcPartAnalyzer &_analyzer)
{
	*this = _analyzer;
}

MtcPartAnalyzer::~MtcPartAnalyzer()
{
	Clear();
}

MtcPartAnalyzer &MtcPartAnalyzer::operator=(MtcPartAnalyzer &_analyzer)
{
	Cluster(_analyzer.Cluster());
	Region(_analyzer.Region());
	KerfedRegion(_analyzer.KerfedRegion());
	Job(_analyzer.Job());
	AvailableArea(_analyzer.AvailableArea());
	ClusterScore(_analyzer.ClusterScore(), 0);
	working_nsg = _analyzer.working_nsg;
	original_nshapes = _analyzer.original_nshapes;
	return (*this);
}

wostream &operator<<(wostream &_stream, MtcPartAnalyzer &_analyzer)
{
	_stream << _T("analyzer [") << &_analyzer << _T("]:") << endl;
	_stream << _T("Cluster: ") << _analyzer.cluster << endl;
	_stream << _T("region: ") << _analyzer.region << endl;
	_stream << _T("kerfed region: ") << _analyzer.kerfed_region << endl;
	_stream << _T("available area: ") << _analyzer.available_area << endl;
	if (_analyzer.cluster_score)
		_stream << _T("cluster_score: ") << *(_analyzer.cluster_score) << endl;
   else if (_analyzer.cluster && _analyzer.cluster->ClusterScore())
	   _stream << _T("cluster_score: ") << *(_analyzer.cluster->ClusterScore()) << endl;
   else
      _stream << _T("No ClusterScore present!") << endl;
	_stream << _T("end of analyzer [") << &_analyzer << _T("]") << endl;
	return (_stream);
}

MtcClusterScore *MtcPartAnalyzer::Analyze(CLUSTER *_cluster, REGION _region, REGION _kerfed_region,
	double _available_area, JOB *_job, MtcClusterScore *_clsscore, MtcClusterScore *_similar_part_score)
{

	Set(_cluster, _region, _kerfed_region, _available_area, _job, _clsscore);

	// don't bother rescoring if the cluster's quantity and the scoring region is identical to what's in the cluster score
	if (Cluster() && ClusterScore() && ClusterScore()->ScoredQuantity() == Cluster()->get_quantity() &&
		ClusterScore()->ScoringRegion() == _region)
		return (ClusterScore());

	OneTimeAnalysis(_similar_part_score);

	// Some things need to be computed every time...
	AnalyzeBigness();
	AnalyzeInterlockability();
	AnalyzeTotalFootprint();
	AnalyzeClcArrayScore();
	AnalyzeProfileNestingScore();
	AnalyzePatternArrayability();
	AnalyzeFilleriness();
	AnalyzeExteriorArcs();

	// Some things only need to be computed if they are needed
	AnalyzeRotatability(_similar_part_score);
   if (Cluster() && ClusterScore())
   {
      ClusterScore()->ScoredQuantity(Cluster()->get_quantity());
      ClusterScore()->SimilarPartsQuantity(Cluster()->get_quantity(defaults.get_nest_parameters().IncludeSimilarParts()));
      ClusterScore()->ScoringRegion(Region());
   }

	return (ClusterScore());
}

void MtcPartAnalyzer::OneTimeAnalysis(MtcClusterScore *_similar_part_score)
{
	if (!Cluster())
		return;

	if (!ClusterScore())
   {
		Cluster()->ClusterScore(new MtcClusterScore(Cluster()));
      cluster_score = Cluster()->ClusterScore();
   }

	if (!ClusterScore()->Initialized())
	{
		if (_similar_part_score)
		{
			ClusterScore()->MinNonLeadsRegion(_similar_part_score->MinNonLeadsRegion());
		}
		else
		{
			REGION
				mnl_region(Cluster()->MinNonLeadsRegion());

			if (!mnl_region.fits_inside(this->Region()))
			{
				mnl_region = Cluster()->MinNonLeadsRegionThatFitsInside(this->Region());
				if (mnl_region.IsEmpty())
					mnl_region = Cluster()->MinNonLeadsRegion();
			}
			ClusterScore()->MinNonLeadsRegion(mnl_region);
		}
		ClusterScore()->ExteriorProfileArea(Cluster()->ExteriorProfileArea(FALSE, 0.0));
		AnalyzeShapeAndTendency();
		AnalyzeRectangularishness();
		AnalyzeClcArrayability();
		AnalyzePatternArrayStrategy();
   	AnalyzeFillerRotation();

		AnalyzeEdginess();
		AnalyzeRighteousness();
		AnalyzeCorneriness();
		AnalyzeSlopiness();
		AnalyzeConcavityOrientation();

		InitialRotatabilityComputation(_similar_part_score);

		ClusterScore()->Initialized(TRUE);
	}
}

MtcNestShape *MtcPartAnalyzer::GetNestShape()
{
	NEST_PART
		*np;
	MtcProfileShape
		*pshape;
	MtcNestShape
		*nshape(0);
	MtcClusterState
		clState;
	MtcProfileShapeState
		psState;
	if (Cluster() && Cluster()->get_num_nest_parts() == 1)
	{
		np = Cluster()->get_first_part(&clState);
		pshape = np->get_part()->ProfileShape(TRUE);
		if (pshape)
			nshape = pshape->NestShape(&psState);
	}
	return (nshape);
}

void MtcPartAnalyzer::AnalyzeShapeAndTendency()
{
	if (!Cluster() || !ClusterScore())
		return;
	ClusterScore()->Tendency(NS_UNKNOWN);
	ClusterScore()->ShapeType(NS_UNKNOWN);
	if (Cluster()->get_num_nest_parts() == 1)
	{
		MtcNestShape
			*nshape = GetNestShape();
		if (nshape)
		{
			if (nshape->Type() == NS_UNKNOWN)
			{
				MtcNestShape
					tshape1,
					*newshape;
				MtcNestShapeState
					nsState;
				MtcNestShapeFactory
					factory;

				tshape1 = *nshape;
				tshape1.Smooth(&nsState, 0.1, 2.0); // smooth it pretty roughly to see if it is close to something
				newshape = factory.CreateNestShape(&tshape1);
				if (newshape)
				{
					ClusterScore()->ShapeType(newshape->Type());
					ClusterScore()->Tendency(newshape->Tendency());
					delete (newshape);
				}
			}
			else
			{
				ClusterScore()->ShapeType(nshape->Type());
				ClusterScore()->Tendency(nshape->Tendency());
			}
		}
	}
}

void MtcPartAnalyzer::Clear()
{
	Set(0, REGION(), REGION(), 0.0, 0);
   this->cluster_score = 0;
	this->working_nsg.Clear();
	this->original_nshapes.Clear();
}

void MtcPartAnalyzer::Set(CLUSTER *_cluster, REGION _region, REGION _kerfed_region,
	double _available_area, JOB *_job, MtcClusterScore *_clsscore)
{
	Cluster(_cluster);
	Region(_region);
	KerfedRegion(_kerfed_region);
   Job(_job);
	AvailableArea(_available_area);
   if (_clsscore)
	   ClusterScore(_clsscore, 0);
   else if (Cluster())
      this->cluster_score = Cluster()->ClusterScore();
}

void MtcPartAnalyzer::AnalyzeBigness()
{
	// Don't need to re-score if region stays the same 
	if (!Cluster() || !ClusterScore() || ClusterScore()->ScoringRegion() == Region())
		return;
	else if (Region().IsEmpty())
		ClusterScore()->BignessScore(1); // used to be 0, but caused divide by 0 error in NEXPERT.CPP
	else
	{
		REGION
			min_region = ClusterScore()->MinNonLeadsRegion();
		ClusterScore()->ProfileRatio(ClusterScore()->ExteriorProfileArea() / AvailableArea());
		ClusterScore()->RegionRatio(min_region.get_area() / AvailableArea());
		ClusterScore()->MaxDimensionRatio(min_region.get_max_dimension() / Region().get_max_dimension());
		ClusterScore()->MinDimensionRatio(min(min_region.get_height(), min_region.get_width()) / min (Region().get_height(), Region().get_width()));
		// Removed dimension ratios from contention
		ClusterScore()->BignessScore((int)(max(ClusterScore()->ProfileRatio(), max(ClusterScore()->RegionRatio(), 0.01)) * 100.0)); // min value is 1 due to possible divide by 0 in NEXPERT.CPP
	}
}

void MtcPartAnalyzer::AnalyzeExteriorArcs()
{
	// Don't need to re-score if region stays the same 
	if (!Cluster() || !ClusterScore() || ClusterScore()->ScoringRegion() == Region())
		return;
	else
	{
		MtcNestShape
			*nshape;
		MtcItemState
			*state;

		nshape = GetNestShape();
		if (nshape && (state = nshape->CreateNewState()))
		{
			MtcNestMotion
				*motion;
			MtcNestArc
				*arc;
			double
				small_threshold,
				medium_threshold,
				large_threshold,
				very_large_threshold,
				max_dimension,
				perimeter;
			CS_ARC_TYPE
				type;
			CS_ARC_SIZE_TYPE
				size;

			//
			// set the threshold values
			//
			max_dimension = Region().get_max_dimension();
			very_large_threshold = 0.5 * max_dimension;
			large_threshold = 0.25 * max_dimension;
			medium_threshold = 0.1 * max_dimension;
			small_threshold = 0.05 * max_dimension;

			//
			// initialize the values
			//
			for (int arctype = CS_CONVEX_ARC; arctype <= CS_CONCAVE_ARC; arctype++)
			{
				for (int arcsize = CS_VERY_SMALL; arcsize <= CS_VERY_LARGE; arcsize++)
				{
					ClusterScore()->ArcsSetNumber((CS_ARC_TYPE)arctype,
						(CS_ARC_SIZE_TYPE)arcsize, 0);
					ClusterScore()->ArcsSetTotalLength((CS_ARC_TYPE)arctype,
						(CS_ARC_SIZE_TYPE)arcsize, 0.0);
					ClusterScore()->ArcsSetPerimeterRatio((CS_ARC_TYPE)arctype,
						(CS_ARC_SIZE_TYPE)arcsize, 0.0);
				}
			}

			//
			// accumulate number and total_length
			//
			motion = nshape->FirstMotion(state);
			while (motion)
			{
				if (motion->IsArc())
				{
					arc = (MtcNestArc *)motion;
					if (arc->InConcavity())
						type = CS_CONCAVE_ARC;
					else
						type = CS_CONVEX_ARC;

					if (arc->Length() > very_large_threshold)
						size = CS_VERY_LARGE;
					else if (arc->Length() > large_threshold)
						size = CS_LARGE;
					else if (arc->Length() > medium_threshold)
						size = CS_MEDIUM;
					else if (arc->Length() > small_threshold)
						size = CS_SMALL;
					else
						size = CS_VERY_SMALL;
					ClusterScore()->ArcsSetNumber(type, size, ClusterScore()->ArcsNumber(type, size) + 1);
					ClusterScore()->ArcsSetTotalLength(type, size, 
						ClusterScore()->ArcsTotalLength(type, size) + arc->Length());
				}
				motion = nshape->NextMotion(state);
			}

			//
			// calculate the perimeter_ratios
			//
			perimeter = nshape->Perimeter();
			if (perimeter > ACCY)
			{
				for (int arctype = CS_CONVEX_ARC; arctype <= CS_CONCAVE_ARC; arctype++)
				{
					for (int arcsize = CS_VERY_SMALL; arcsize <= CS_VERY_LARGE; arcsize++)
					{
						ClusterScore()->ArcsSetPerimeterRatio((CS_ARC_TYPE)arctype,(CS_ARC_SIZE_TYPE)arcsize, 
							ClusterScore()->ArcsTotalLength((CS_ARC_TYPE)arctype,(CS_ARC_SIZE_TYPE)arcsize) / perimeter); 
					}
				}
			}

			//
			// clean up
			//
			delete (state);
		}
	}
}

void MtcPartAnalyzer::AnalyzeRectangularishness()
{
	if (!Cluster() || !ClusterScore())
		return;
	else
	{
		REGION
			min_region = ClusterScore()->MinNonLeadsRegion();
		double
			score(0.0),
			// precedent set for this calc in ANESTGEN.CPP BuildRectangularClusters
			profile_area = Cluster()->ExteriorProfileArea(TRUE, 2.0);

		score = ((profile_area / min_region.get_area()) * 100.0) + 0.5;
		ClusterScore()->RectangularishnessScore((int)score);
	}
}

void MtcPartAnalyzer::AnalyzeInterlockability()
{
	if (!Cluster() || !ClusterScore())
		return;
	else
	{
      MTC_NEST_SHAPE_TYPE
         nstype;
		int
			iscore;

		if (ClusterScore()->ShapeType() == NS_UNKNOWN)
			nstype = ClusterScore()->Tendency();
		else
			nstype = ClusterScore()->ShapeType();

		switch (nstype)
		{
			case NS_L_BRACKET:
			case NS_U_BRACKET:
			case NS_ROUNDED_U_BRACKET:
			case NS_CONVEX_QUADRILATERAL_90:
			case NS_RIGHT_TRIANGLE:
			case NS_TRIANGLE:
			case NS_QUADRILATERAL:
			case NS_ARC_SECTION:
			case NS_OFFSET_ARC_SECTION:
			case NS_ACUTE_WEDGE:
			case NS_90_WEDGE:
			case NS_PIE_MISSING_OBTUSE_SLICE:
			case NS_TEAR_DROP:
			case NS_DUMBBELL:
			case NS_INVERTED_90_WEDGE:
			case NS_INVERTED_ACUTE_WEDGE:
			case NS_INVERTED_OBTUSE_WEDGE:
			case NS_INVERTED_TOMBSTONE_RECTANGLE:
			case NS_4_NOTCHED_CORNER_RECTANGLE:
			case NS_3_NOTCHED_CORNER_RECTANGLE:
			case NS_2_NOTCHED_CORNER_RECTANGLE:
			case NS_1_NOTCHED_CORNER_RECTANGLE:
			case NS_SHARKFIN:
			case NS_T_SHAPE:
				// Shapes with excellent interlocking
				iscore = 100;
				break;
			case NS_CIRCLE: 
			case NS_PENTAGON:
			case NS_HEXAGON:
			case NS_OCTAGON:
			case NS_TRAPEZOID:
			case NS_PARALLELOGRAM:
			case NS_OBTUSE_WEDGE:
			case NS_SHEAVE:
			case NS_PIE_MISSING_QUARTER_SLICE:
			case NS_PIE_MISSING_ACUTE_SLICE:
			case NS_MAJOR_FLAT_SIDED_CIRCLE:
			case NS_PADDLE:
			case NS_SLOTTED_CIRCLE:
			case NS_ARC_TRAPEZOID:
			case NS_ELLIPSE:
				// Shapes with pretty good interlocking possibilities
				iscore = 85;
				break;
			case NS_OBROUND:
			case NS_SQUIRCLE:
			case NS_RECTIRCLE:
			case NS_SEMI_CIRCLE:
			case NS_MINOR_FLAT_SIDED_CIRCLE:
				// Shapes with even less possibility of interlocking
				iscore = 75;
				break;
			case NS_4_ROUNDED_CORNER_RECTANGLE:
			case NS_3_ROUNDED_CORNER_RECTANGLE:
			case NS_2_ROUNDED_CORNER_RECTANGLE:
			case NS_1_ROUNDED_CORNER_RECTANGLE:
			case NS_TOMBSTONE_RECTANGLE:
				iscore = 70;
				break;
			case NS_SQUARE:
			case NS_RECTANGLE:
				// Shapes with little or no interlocking
				iscore = 60;
				break;
			case NS_UNKNOWN: 
			default:
				// percentage of profile region area actually occupied by exterior profile
				double
					ratio(ClusterScore()->ProfileRatio()/ClusterScore()->RegionRatio());

				iscore = (int)((ratio * Weights()->MaterialUsedFactor()) + ((1 - ratio) * Weights()->MaterialMissingFactor()));
				MtcNestShape
					*nshape = GetNestShape();
				MtcNestShapeState
					nsState;
				if (nshape && nshape->FirstConcavity(&nsState))
				{
					// Extra points for having concavities - the larger the better
					MtcNestShape
						*concavity = nshape->FirstConcavity(&nsState);
               double
               	accum(0.0);
                  
					while (concavity)
					{
						accum += concavity->Area();
						concavity = nshape->NextConcavity(&nsState);
					}
					accum /= ClusterScore()->MinNonLeadsRegion().Area();
					if (accum > 1.0)
						accum = 1.0;
					iscore += (int)(accum * Weights()->ConcavityFactor());
				}
				break;
		}
		ClusterScore()->InterlockabilityScore(iscore);
	}
}

void MtcPartAnalyzer::AnalyzeTotalFootprint()
{
	if (!Cluster() || !ClusterScore())
		return;
	else
	{
		int
			rows,
			cols,
			rows90,
			cols90,
			quantity;
		rows = (int)(Region().Height() / ClusterScore()->MinNonLeadsRegion().Height());
		cols = (int)(Region().Width() / ClusterScore()->MinNonLeadsRegion().Width());
		rows90 = (int)(Region().Height() / ClusterScore()->MinNonLeadsRegion().Width());
		cols90 = (int)(Region().Width() / ClusterScore()->MinNonLeadsRegion().Height());
		quantity = max(rows * cols, rows90 * cols90);

		if (quantity > Cluster()->get_quantity(defaults.get_nest_parameters().IncludeSimilarParts()))
			quantity = Cluster()->get_quantity(defaults.get_nest_parameters().IncludeSimilarParts());
		ClusterScore()->TotalProfileSimilarPartsFootprint(ClusterScore()->ExteriorProfileArea() * quantity);
		ClusterScore()->TotalRegionSimilarPartsFootprint(ClusterScore()->MinNonLeadsRegion().get_area() * quantity);
		ClusterScore()->EstimatedSimilarPartsFootprintQuantity(quantity);

		if (defaults.get_nest_parameters().IncludeSimilarParts() && quantity > Cluster()->get_quantity())
			quantity = Cluster()->get_quantity();
		ClusterScore()->TotalProfileFootprint(ClusterScore()->ExteriorProfileArea() * quantity);
		ClusterScore()->TotalRegionFootprint(ClusterScore()->MinNonLeadsRegion().get_area() * quantity);
		ClusterScore()->EstimatedFootprintQuantity(quantity);
	}
}

void MtcPartAnalyzer::AnalyzeClcArrayability()
{
   // Also, if quantity comes into play, need to have a general CLC Array
   //    property and one modified based on quantity
	if (!ClusterScore())
		return;

   ClusterScore()->ClcArrayable(0);
	if (Cluster() && Cluster()->get_num_nest_parts() == 1)
	{
		MtcClcGenerator
			clcgen(Job());
      MTC_NEST_SHAPE_TYPE
         shape_type = NS_UNKNOWN;

		// Is it even possible to CLC array the cluster?
		ClusterScore()->ClcArrayable(clcgen.CanClcArray(Cluster(), 0, 0, &shape_type));
		if (shape_type != NS_UNKNOWN)
      	ClusterScore()->ShapeType(shape_type);
	}
}

void MtcPartAnalyzer::AnalyzeClcArrayScore()
{
	if (!ClusterScore())
		return;

	ClusterScore()->ClcArrayabilityScore(0);

	// Does it make sense to try arraying it? (Must be a single part cluster taking up at most half of the available region)
	if (Cluster() && Cluster()->get_num_nest_parts() == 1 && ClusterScore()->ClcArrayable()
		&& ClusterScore()->EstimatedFootprintQuantity() > 1 && (ClusterScore()->ProfileRatio() - 0.50) < ACCY)
	{
		double
			factor(1.0);
		int
			qty(ClusterScore()->EstimatedFootprintQuantity());
      // Does not take full vs. partial columns into account, nor the actual
      //    quantity that will be arrayed...
		MtcClcGenerator
			clcgen(Job());
      double
         score = (ClusterScore()->ProfileRatio() / ClusterScore()->RegionRatio()) * 100.0;
      if (score > 100.0 || !clcgen.IsGeneralClcArrayShape(ClusterScore()->ShapeType()))
         score = 100.0;
		if (qty <= 2)
			factor = 0.25;
		if (qty <= 4)
			factor = 0.50;
		if (qty <= 6)
			factor = 0.75;
		score *= factor;

      ClusterScore()->ClcArrayabilityScore((int)score);
	}
}

void MtcPartAnalyzer::AnalyzePatternArrayability()
{
   // If quantity comes into play, need to have a general Pattern Array
   //    property and one modified based on quantity
	double
		base_score,
#ifdef OLD_SCORES
		q_score,
#endif
		qscore_modifier(100.0),
		accum(0.0),
		score(UNDEFINED_SCORE); 
	if (Cluster() && ClusterScore() && ClusterScore()->EstimatedSimilarPartsFootprintQuantity() > 1 && (ClusterScore()->ProfileRatio() - 0.50) < ACCY)
	{
      MTC_NEST_SHAPE_TYPE
         nstype;
		if (ClusterScore()->ShapeType() == NS_UNKNOWN)
			nstype = ClusterScore()->Tendency();
		else
			nstype = ClusterScore()->ShapeType();

		switch (nstype)
		{
			case NS_L_BRACKET:
			case NS_U_BRACKET:
			case NS_ROUNDED_U_BRACKET:
			case NS_CONVEX_QUADRILATERAL_90:
			case NS_RIGHT_TRIANGLE:
			case NS_TRIANGLE:
			case NS_QUADRILATERAL:
			case NS_ARC_SECTION:
			case NS_OFFSET_ARC_SECTION:
			case NS_ACUTE_WEDGE:
			case NS_90_WEDGE:
			case NS_PIE_MISSING_OBTUSE_SLICE:
			case NS_TEAR_DROP:
			case NS_DUMBBELL:
			case NS_INVERTED_90_WEDGE:
			case NS_INVERTED_ACUTE_WEDGE:
			case NS_INVERTED_OBTUSE_WEDGE:
			case NS_INVERTED_TOMBSTONE_RECTANGLE:
			case NS_4_NOTCHED_CORNER_RECTANGLE:
			case NS_3_NOTCHED_CORNER_RECTANGLE:
			case NS_2_NOTCHED_CORNER_RECTANGLE:
			case NS_1_NOTCHED_CORNER_RECTANGLE:
			case NS_SHARKFIN:
			case NS_T_SHAPE:
				// Shapes with excellent interlocking
				base_score = 100;
				break;
			case NS_CIRCLE: 
			case NS_PENTAGON:
			case NS_HEXAGON:
			case NS_OCTAGON:
			case NS_TRAPEZOID:
			case NS_PARALLELOGRAM:
			case NS_OBTUSE_WEDGE:
			case NS_SHEAVE:
			case NS_PIE_MISSING_QUARTER_SLICE:
			case NS_PIE_MISSING_ACUTE_SLICE:
			case NS_MAJOR_FLAT_SIDED_CIRCLE:
			case NS_PADDLE:
			case NS_SLOTTED_CIRCLE:
			case NS_ARC_TRAPEZOID:
			case NS_ELLIPSE:
				// Shapes with pretty good interlocking possibilities
				base_score = 85;
				break;
			case NS_OBROUND:
			case NS_SQUIRCLE:
			case NS_RECTIRCLE:
			case NS_SEMI_CIRCLE:
			case NS_MINOR_FLAT_SIDED_CIRCLE:
				// Shapes with even less possibility of interlocking
				base_score = 75;
				break;
			case NS_4_ROUNDED_CORNER_RECTANGLE:
			case NS_3_ROUNDED_CORNER_RECTANGLE:
			case NS_2_ROUNDED_CORNER_RECTANGLE:
			case NS_1_ROUNDED_CORNER_RECTANGLE:
			case NS_TOMBSTONE_RECTANGLE:
				base_score = 70;
				break;
			case NS_SQUARE:
			case NS_RECTANGLE:
				// Shapes with little or no interlocking
				base_score = 60;
				break;
			case NS_UNKNOWN: 
			default:
				// percentage of profile region area actually occupied by exterior profile
				double
					ratio(ClusterScore()->ProfileRatio()/ClusterScore()->RegionRatio());

				base_score = (ratio * Weights()->MaterialUsedFactor()) + ((1 - ratio) * Weights()->MaterialMissingFactor());
				MtcNestShape
					*nshape = GetNestShape();
				MtcNestShapeState
					nsState;
				if (nshape && nshape->FirstConcavity(&nsState))
				{
					// Extra points for having concavities - the larger the better
					MtcNestShape
						*concavity = nshape->FirstConcavity(&nsState);
					while (concavity)
					{
						accum += concavity->Area();
						concavity = nshape->NextConcavity(&nsState);
					}
					accum /= ClusterScore()->MinNonLeadsRegion().Area();
					if (accum > 1.0)
						accum = 1.0;
					base_score += accum * Weights()->ConcavityFactor();
				}
				break;
		}
#ifdef OLD_SCORES
		if (ClusterScore()->EstimatedFootprintQuantity() <= 5)
			qscore_modifier = 25.0 * (ClusterScore()->EstimatedFootprintQuantity() - 1);
		q_score = qscore_modifier * ClusterScore()->RegionRatio() * ClusterScore()->EstimatedFootprintQuantity();
		score = (q_score + base_score) / 2.0;
		ClusterScore()->PatternArrayabilityScore((int)score);
#else
		// alternate patternarrayability score; use with alternate profile nesting score and alternate PRATIO values
		int
			eqty = ClusterScore()->EstimatedFootprintQuantity();
		score = (100 - (eqty * ClusterScore()->ProfileNestingScore() / (eqty + 1))) * eqty/(eqty + 2);
		ClusterScore()->PatternArrayabilityScore((int)score); 
#endif
	}
}

void MtcPartAnalyzer::AnalyzePatternArrayStrategy()
{
   // If quantity comes into play, need to have a general Pattern Array
   //    property and one modified based on quantity
	if (!Cluster() || !ClusterScore())
		return;
	else
	{
		int
			strategy(0),
			angle(90);
		if (ClusterScore()->RectangularishnessScore() > 90)
		{
			strategy = BASIC_PATTERN;
			angle = 90;
		}
		else if (Cluster()->get_num_nest_parts() == 1)
		{
			MTC_NEST_SHAPE_TYPE
				nstype(ClusterScore()->ShapeType());
			if (nstype == NS_UNKNOWN)
				nstype = ClusterScore()->Tendency();
			switch (nstype)
			{
				case NS_CIRCLE: 
				case NS_L_BRACKET:
				case NS_U_BRACKET:
				case NS_ROUNDED_U_BRACKET:
				case NS_PENTAGON:
				case NS_CONVEX_QUADRILATERAL_90:
				case NS_OBTUSE_WEDGE:
				case NS_SHEAVE:
				case NS_PIE_MISSING_QUARTER_SLICE:
				case NS_PIE_MISSING_ACUTE_SLICE:
				case NS_MAJOR_FLAT_SIDED_CIRCLE:
				case NS_PADDLE:
				case NS_SLOTTED_CIRCLE:
				case NS_ELLIPSE:
					angle = 45;
					strategy = INTERMEDIATE_PATTERN;
					break;
				case NS_HEXAGON:
				case NS_OCTAGON:
				case NS_RIGHT_TRIANGLE:
				case NS_OBROUND:
				case NS_SQUIRCLE:
				case NS_RECTIRCLE:
				case NS_SQUARE:
				case NS_RECTANGLE:
				case NS_TRAPEZOID:
				case NS_PARALLELOGRAM:
				case NS_4_ROUNDED_CORNER_RECTANGLE:
				case NS_3_ROUNDED_CORNER_RECTANGLE:
				case NS_2_ROUNDED_CORNER_RECTANGLE:
				case NS_1_ROUNDED_CORNER_RECTANGLE:
				case NS_90_WEDGE:
				case NS_INVERTED_90_WEDGE:
				case NS_SEMI_CIRCLE:
				case NS_MINOR_FLAT_SIDED_CIRCLE:
				case NS_TOMBSTONE_RECTANGLE:
				case NS_ARC_TRAPEZOID:
				case NS_4_NOTCHED_CORNER_RECTANGLE:
				case NS_3_NOTCHED_CORNER_RECTANGLE:
				case NS_2_NOTCHED_CORNER_RECTANGLE:
				case NS_1_NOTCHED_CORNER_RECTANGLE:
				case NS_T_SHAPE:
					angle = 90;
					strategy = BASIC_PATTERN;
					break;
				case NS_DUMBBELL:
				case NS_INVERTED_TOMBSTONE_RECTANGLE:
					angle = 90;
					strategy = INTERMEDIATE_PATTERN;
					break;
				case NS_TRIANGLE:
				case NS_QUADRILATERAL:
				case NS_ARC_SECTION:
				case NS_OFFSET_ARC_SECTION:
				case NS_ACUTE_WEDGE:
				case NS_PIE_MISSING_OBTUSE_SLICE:
				case NS_TEAR_DROP:
				case NS_INVERTED_ACUTE_WEDGE:
				case NS_INVERTED_OBTUSE_WEDGE:
				case NS_SHARKFIN:
					angle = 15;
					strategy = ADVANCED_PATTERN;
					break;
				case NS_UNKNOWN: 
				default:
					MtcNestShape
						*nshape = GetNestShape();
					if (nshape)
					{
						int
							lines(nshape->NumberOfLines()),
							arcs(nshape->NumberOfArcs()),
							concavities(nshape->Concavities());
						if (arcs == 2 && lines == 2)  // Guess at an arc section
						{
							strategy = ADVANCED_PATTERN;
							angle = 15;
						}
						else if (arcs == 1 && lines == 2 && concavities == 0) // ice-cream cone shape
						{
							strategy = BASIC_PATTERN;
							angle = 90;
						}
						else if (arcs == 1 && lines == 2 && concavities >= 1) // Pac-man shape
						{
							strategy = INTERMEDIATE_PATTERN;
							angle = 45;
						}
						else if (concavities == 1)
						{
							strategy = INTERMEDIATE_PATTERN;
							angle = 45;
						}
						else if (concavities > 1)
						{
							strategy = ADVANCED_PATTERN;
							if (lines + arcs < 10)
								angle = 45;
							else if (lines + arcs < 20)
								angle = 30;
							else if (lines + arcs < 30)
								angle = 15;
							else if (lines + arcs < 50)
								angle = 10;
						}
						else
						{
							strategy = BASIC_PATTERN;
							if (lines + arcs < 15)
								angle = 90;
							else if (lines + arcs < 25)
								angle = 45;
							else if (lines + arcs < 35)
								angle = 30;
							else if (lines + arcs < 50)
								angle = 15;
							else
								angle = 10;
						}
					}
					break;
			}
		}
		else
		{	
			// Multi-part cluster - will need to revisit this later...
			strategy = INTERMEDIATE_PATTERN;
			// see what percentage of min region is occupied by cluster and assign angle accordingly...
			// (the less a part occupies the region, the more rotations will be tried)
			REGION
				min_region = ClusterScore()->MinNonLeadsRegion();
			double
				temp = ClusterScore()->ExteriorProfileArea() * 100.0 / min_region.get_area();
			if (temp > 85)
				angle = 45;
			else if (temp > 75)
				angle = 30;
			else if (temp > 65)
				angle = 15;
			else if (temp > 55)
				angle = 10;
			else if (temp > 45)
				angle = 5;
			else
				angle = 1;
		}
		ClusterScore()->PatternArrayStrategy(strategy);
		ClusterScore()->PatternArrayRotation(angle);
	}
}

void MtcPartAnalyzer::InitialRotatabilityComputation(MtcClusterScore *_similar_part_score)
{
	if (!Cluster() || !ClusterScore())
		return;

	int
		i;

	if (_similar_part_score)
	{
		ClusterScore()->SetRotationValue(0, _similar_part_score->GetRotationValue(0.0));
		for(i = (int)LARGE_ANGLE_STEP; i < 90; i+= (int)LARGE_ANGLE_STEP)
			ClusterScore()->SetRotationValue(i, _similar_part_score->GetRotationValue(i));
	}
	else
	{
		CLUSTER
			temp(*Cluster());
	
	   temp.rotate(0, temp.get_center());
		ClusterScore()->SetRotationValue(0, temp.get_kerfed_non_leads_region());
		for(i = (int)LARGE_ANGLE_STEP; i < 90; i+= (int)LARGE_ANGLE_STEP)
		{
			temp.rotate(LARGE_ANGLE_STEP, temp.get_center());
			ClusterScore()->SetRotationValue(i, temp.get_kerfed_non_leads_region());
		}
	}
}

void MtcPartAnalyzer::AnalyzeRotatability(MtcClusterScore *_similar_part_score)
{
	// Don't need to recompute if region stays the same
	if (!Cluster() || !ClusterScore() || Region() == ClusterScore()->ScoringRegion())
		return;
	else if (_similar_part_score)
	{
		ClusterScore()->MaximizeHeightAngle(_similar_part_score->MaximizeHeightAngle());
		ClusterScore()->MaximizeWidthAngle(_similar_part_score->MaximizeWidthAngle());
	}
	else if (Region().IsEmpty())
   {
		ClusterScore()->MaximizeHeightAngle(UNDEFINED_SCORE);
		ClusterScore()->MaximizeWidthAngle(UNDEFINED_SCORE);
   }
	else
	{
		const DOUBLE
			SAFETY_MARGIN = 0.005;

		ClusterScore()->MaximizeHeightAngle(RegionAngleForLength(KerfedRegion().Height() - SAFETY_MARGIN, 1));
		ClusterScore()->MaximizeWidthAngle(RegionAngleForLength(KerfedRegion().Width() - SAFETY_MARGIN, 0));
	}
}

double MtcPartAnalyzer::RegionAngleForLength(double _length, int _checking_height)
{
   // Should this take the cluster's rotation restriction angle into account?
	double
		i,
		start_index,
		end_index,
		using_height(0),
		save_index(-1);
	double
		distance(9999999999.9),
		l1,
		l2,
		d,
		save_angle(-1.0);
   CLUSTER
      temp;
   REGION
      tregion;

	if (!ClusterScore())
		return (save_angle);
			
	for (i = 0; i < 90; i += LARGE_ANGLE_STEP)
	{
		tregion = ClusterScore()->GetRotationValue(i);
		if (tregion.is_empty())
		{
			temp = *Cluster();
		   temp.rotate(i, temp.get_center());

			// modified all regions to use kerfed regions INCLUDING leads...the original
			// logic was using non-leads regions and this was leading to calculation of
			// rotation angles where the part couldn't fit in the region
			// 
			// tregion = temp.NonLeadsRegion();
			tregion = temp.get_kerfed_region();
			ClusterScore()->SetRotationValue(i, tregion);
		}
		if (!tregion.is_empty())
		{
			l1 = _length - tregion.get_height();
			l2 = _length - tregion.get_width();
			if (l1 > -ACCY && l1 - distance < -ACCY)
			{
				distance = l1;
				save_index = i;
				if (fabs(l1 - l2) < ACCY)
					using_height = _checking_height;
				else
					using_height = 1;
			}
			if (l2 > -ACCY && l2 - distance < -ACCY)
			{
				distance = l2;
				save_index = i;
				if (fabs(l1 - l2) < ACCY)
					using_height = _checking_height;
				else
					using_height = 0;
			}
		}
	}
	if (save_index > -1)
	{
		if (save_index >= LARGE_ANGLE_STEP)
			start_index = (save_index - LARGE_ANGLE_STEP);
		else
			start_index = (save_index + 90 - LARGE_ANGLE_STEP);
		end_index = start_index + 2 * LARGE_ANGLE_STEP;
		for(d = start_index; d <= end_index; d += MEDIUM_ANGLE_STEP)
		{
			tregion = ClusterScore()->GetRotationValue(d);
			if (tregion.is_empty())
			{
   			temp = *Cluster();
			   temp.rotate(d, temp.get_center());
				tregion = temp.get_kerfed_region();
				ClusterScore()->SetRotationValue(d, tregion);
			}
			if (!tregion.is_empty())
			{
				l1 = _length - tregion.get_height();
				l2 = _length - tregion.get_width();
				if (l1 > -ACCY && l1 - distance < -ACCY)
				{
					distance = l1;
					save_index = d;
					if (fabs(l1 - l2) < ACCY)
						using_height = _checking_height;
					else
						using_height = 1;
				}
				if (l2 > -ACCY && l2 - distance < -ACCY)
				{
					distance = l2;
					save_index = d;
					if (fabs(l1 - l2) < ACCY)
						using_height = _checking_height;
					else
						using_height = 0;
				}
			}
		}
		if (save_index >= MEDIUM_ANGLE_STEP)
			start_index = (save_index - MEDIUM_ANGLE_STEP);
		else
			start_index = (save_index + 90 - MEDIUM_ANGLE_STEP);
		end_index = start_index + 2 * MEDIUM_ANGLE_STEP;
		for(d = start_index; d <= end_index; d += SMALL_ANGLE_STEP)
		{
			tregion = ClusterScore()->GetRotationValue(d);
			if (tregion.is_empty())
			{
   			temp = *Cluster();
			   temp.rotate(d, temp.get_center());
				tregion = temp.get_kerfed_region();
				ClusterScore()->SetRotationValue(d, tregion);
			}
			if (!tregion.is_empty())
			{
				l1 = _length - tregion.get_height();
				l2 = _length - tregion.get_width();
				if (l1 > -ACCY && l1 - distance < -ACCY)
				{
					distance = l1;
					save_index = d;
					if (fabs(l1 - l2) < ACCY)
						using_height = _checking_height;
					else
						using_height = 1;
				}
				if (l2 > -ACCY && l2 - distance < -ACCY)
				{
					distance = l2;
					save_index = d;
					if (fabs(l1 - l2) < ACCY)
						using_height = _checking_height;
					else
						using_height = 0;
				}
			}
		}
		if (save_index >= SMALL_ANGLE_STEP)
			start_index = (save_index - SMALL_ANGLE_STEP);
		else
			start_index = (save_index + 90 - SMALL_ANGLE_STEP);
		end_index = start_index + 2 * SMALL_ANGLE_STEP;
		for(d = start_index; d <= end_index; d += SMALLEST_ANGLE_STEP)
		{
			tregion = ClusterScore()->GetRotationValue(d);
			if (tregion.is_empty())
			{
   			temp = *Cluster();
			   temp.rotate(d, temp.get_center());
				tregion = temp.get_kerfed_region();
				ClusterScore()->SetRotationValue(d, tregion);
			}
			if (!tregion.is_empty())
			{
				l1 = _length - tregion.get_height();
				l2 = _length - tregion.get_width();
				if (l1 > -ACCY && l1 - distance < -ACCY)
				{
					distance = l1;
					save_index = d;
					if (fabs(l1 - l2) < ACCY)
						using_height = _checking_height;
					else
						using_height = 1;
				}
				if (l2 > -ACCY && l2 - distance < -ACCY)
				{
					distance = l2;
					save_index = d;
					if (fabs(l1 - l2) < ACCY)
						using_height = _checking_height;
					else
						using_height = 0;
				}
			}
		}
      save_angle = save_index;
		if (using_height != _checking_height)
			save_angle = save_angle + 90.0;
	}
	return (save_angle);
}

void MtcPartAnalyzer::AnalyzeCorneriness()
{
	if (!Cluster() || !ClusterScore())
		return;
	// Must be at least a little righteous (for now) and must not be a multi-part cluster
	// To handle some of the other "cornery" cases, may not be able to test for righteousness by itself.
	else if (ClusterScore()->RighteousnessScore() == 0 || this->original_nshapes.NumberOfItems() != 1)
	{
		ClusterScore()->CornerinessScore(UNDEFINED_SCORE);
		ClusterScore()->CornerinessAngle(UNDEFINED_SCORE);
		return;
	}
	else if (ClusterScore()->ExteriorProfileArea() > (ClusterScore()->MinNonLeadsRegion().Area() * Weights()->CornerinessAreaFactor()))
	{
		ClusterScore()->CornerinessScore(UNDEFINED_SCORE);
		ClusterScore()->CornerinessAngle(UNDEFINED_SCORE);
		return;
	}
	else
	{

		//
		// Using only single part clusters for this, as creating voids for multi-part clusters in not
		//		nearly as easy (or fast) as it is for single part clusters
		//

		MtcNestShape
         *nshape,
			*nshape1;
		MtcNestMotion
			*prev,
			*next;
		double
			angle,
			rot_angle,
			save_area,
			area_accum,
         temp_save_area,
         save_length;
		int
			found(0);
		POINT_2D
			pivot;
		REGION
			nregion;
		MtcNestShapeGroup
			nsgroup;
		MtcNestShapeGroupState
			onsgState,
			nsgState;
		MtcClusterScoreState
			cssState;

		nshape1 = this->original_nshapes.FirstNestShape(&onsgState);
		if (nshape1)
		{
			save_area = 0.0;
         save_length = 0.0;
			found = ClusterScore()->FirstRighteousPair(&prev, &next, angle, &cssState);
			while (found)
			{
				if (fabs(angle - 90.0) < Weights()->RightAngleThreshold() && prev->IsLine() && next->IsLine())
				{
					// Rotate "prev" line so it's on the bottom of the part (this should put the angle in the lower left corner)
					MtcNestLine
						*line;
					if (prev->Length() > next->Length() - ACCY)
					{
						line = dynamic_cast<MtcNestLine *>(prev);
						rot_angle = 180.0 - line->AngleInDegrees();
						pivot = line->End();
					}
					else
					{
						line = dynamic_cast<MtcNestLine *>(next);
						rot_angle = 90.0 - line->AngleInDegrees();
						pivot = line->Start();
					}
					if (rot_angle < -ACCY)
						rot_angle += 360.0;
					nshape1->Rotate(rot_angle, pivot);
					nregion = nshape1->Region();
					// only look for corneriness if nothing exists in the shape at the upper right corner
					if (!nshape1->Within(nregion.UpperRight(), 0.0001))
					{
						CreateVoids(nshape1, nregion, &nsgroup);
						found = 0;
						area_accum = 0.0;
						temp_save_area = 0.0;
						// Check for voids at the upper right corner - can only be cornery if there's a single void
						nshape = nsgroup.FirstNestShape(&nsgState);
						while (nshape)
						{	
							// Keep going while the current void is not extremely close to the upper right of the region
							while (nshape && !nshape->Within(nregion.UpperRight(), 0.0001))
							{
								area_accum += nshape->Area();
								nshape = nsgroup.NextNestShape(&nsgState);
							}
							if (nshape)
							{
								area_accum += nshape->Area();
								temp_save_area += nshape->Area();
								found ++;
								nshape = nsgroup.NextNestShape(&nsgState);
							}
						}
						if (temp_save_area > (save_area * 0.5) && prev->Length() * next->Length() > save_length)
						{
							save_area = temp_save_area;
                     save_length = prev->Length() * next->Length();
							// Multiple concavities lowers the score...
							if (found > 1)
								save_area = save_area * (1 - (found/10));
							ClusterScore()->CornerinessScore((int)(100 * save_area / area_accum));
							ClusterScore()->CornerinessAngle(rot_angle);
						}
					}
					nshape1->Rotate(-rot_angle, pivot);
				}
				found = ClusterScore()->NextRighteousPair(&prev, &next, angle, &cssState);
			}
		}
	}
}

int MtcPartAnalyzer::FixArcsCloseToBoundaries(MtcNestShape *_shape, MtcNestShape *_region_shape, REGION &_region)
{
	int
		error(MTC_ERROR);

	if (!_shape || !_region_shape)
		return (error);

	MtcNestMotion
		*motion,
		*prev,
		*next,
		*newmotion;
	POINT_2D
		st,
		end,
		mid;
	int
		ok,
		at_least_one(0);
	MtcNestShapeState
		nsState,
		rnsState;
	prev = _shape->LastUntransMotion(&nsState);
	motion = _shape->FirstUntransMotion(&nsState);
	while (motion)
	{
		if (motion->IsArc())
		{
			st = motion->Start();
			end = motion->End();
			mid = motion->MidPoint();
			ok = 0;
			//
			// if an arc is entirely within the region boundary...
			//
			if (_region.OnBoundary(st) && _region.OnBoundary(end) && _region.OnBoundary(mid))
			{
				//
				// Make sure its start, end, and midpoints are all within the same side of the region.
				//		If so, set the start and end X or Y values to that side of the region
				//
				if (fabs(_region.XMax() - st.X()) < ACCY && fabs(_region.XMax() - end.X()) < ACCY && fabs(_region.XMax() - mid.X()) < ACCY)
				{
					st.X(_region.XMax());
					end.X(_region.XMax());
					ok = 1;
				}
				else if (fabs(_region.XMin() - st.X()) < ACCY && fabs(_region.XMin() - end.X()) < ACCY && fabs(_region.XMin() - mid.X()) < ACCY)
				{
					st.X(_region.XMin());
					end.X(_region.XMin());
					ok = 1;
				}
				else if (fabs(_region.YMax() - st.Y()) < ACCY && fabs(_region.YMax() - end.Y()) < ACCY && fabs(_region.YMax() - mid.Y()) < ACCY)
				{
					st.Y(_region.YMax());
					end.Y(_region.YMax());
					ok = 1;
				}
				else if (fabs(_region.YMin() - st.Y()) < ACCY && fabs(_region.YMin() - end.Y()) < ACCY && fabs(_region.YMin() - mid.Y()) < ACCY)
				{
					st.Y(_region.YMin());
					end.Y(_region.YMin());
					ok = 1;
				}

				if (ok)
				{
					at_least_one = 1;
					next = _shape->NextUntransMotion(&nsState);
					if (!next)
						next = _shape->FirstUntransMotion(&nsState);
					//
					// Set end value of previous motion and start value of next motion to new start and end values
					//
					if (prev != motion && prev != next && prev->End() == motion->Start())
						prev->Set(prev->Start(), st);
					if (next && next != motion && next != prev && next->Start() == motion->End())
						next->Set(end, next->End());
					//
					// Replace the existing arc with a line and mark it as a common motion
					//
					newmotion = new MtcNestLine(st, end);
					_shape->InsertMotion(newmotion, motion);
					_shape->DeleteMotion(motion, &nsState);
               newmotion->IsCommon(TRUE);
					motion = _shape->CurrentUntransMotion(newmotion, &nsState);
					//
					// Split the equivalent motion in the region shape and remove it
					//
					_region_shape->SplitMotion(motion->Start(), min(motion->Length() / 2.0, ACCY));
					_region_shape->SplitMotion(motion->End(), min(motion->Length() / 2.0, ACCY));
					if (_region_shape->UntransWithin(motion->MidPoint(), min(ACCY, motion->Length() / 2.0), &newmotion))
   					_region_shape->DeleteMotion(newmotion, &rnsState);
            }
         }
      }
		prev = motion;
		motion = _shape->NextUntransMotion(&nsState);
	}
	if (at_least_one)
		_shape->DeleteCommonMotions(&nsState);

	error = MTC_NOERROR;
	return (error);
}

void MtcPartAnalyzer::CreateVoids(MtcNestShape *_shape, REGION _region, MtcNestShapeGroup *_void_group)
{
	if (!_shape || !_void_group || _shape->StartPoint() != _shape->EndPoint())
		return;

	_void_group->Clear();

	MtcNestShape
		region_shape,
		shape(*_shape),
		*new_shape(0);
	MtcNestMotion
		*motion,
		*motion2,
		*untrans_motion;
	int
		first_time,
		done(0);
	int
		nulls(0);
	MtcNestShapeState
		nsState,
		rnsState;

	region_shape.Clear();
	region_shape.AddMotion(new MtcNestLine(_region.LowerLeft(), _region.UpperLeft()));
	region_shape.AddMotion(new MtcNestLine(_region.UpperLeft(), _region.UpperRight()));
	region_shape.AddMotion(new MtcNestLine(_region.UpperRight(), _region.LowerRight()));
	region_shape.AddMotion(new MtcNestLine(_region.LowerRight(), _region.LowerLeft()));
	region_shape.TransformSelf();
	shape.Reverse(&nsState);
	shape.TransformSelf();
	region_shape.StartProfileOp(_shape->StartProfileOp());
	region_shape.TorchOnOp(_shape->TorchOnOp());
	region_shape.MarkCommonMotions(&shape);
	shape.MarkCommonMotions(&region_shape);
	region_shape.DeleteCommonMotions(&rnsState);
	shape.DeleteCommonMotions(&nsState);
	region_shape.SplitAtIntersectionsWith(&shape, ACCY/10.0, ACCY/10.0);
	FixArcsCloseToBoundaries(&shape, &region_shape, _region);
	motion = shape.FirstMotion(&nsState);
	while (motion)
	{
		// look for a motion that starts on the region boundary
		if (_region.OnBoundary(motion->Start()))
		{
			untrans_motion = shape.CurrentUntransMotion(&nsState);
			if (!new_shape)
				new_shape = new MtcNestShape();
			done = 0;
			first_time = 1;
			while (!done)
			{
				new_shape->AddMotion(motion->NewCopy());
				shape.CurrentUntransMotion(&nsState)->IsCommon(TRUE);
				done = _region.OnBoundary(motion->End());
			    if (!done)
				   motion = shape.NextMotion(&nsState);
				if (!motion && first_time)
				{
					motion = shape.FirstMotion(&nsState);
					first_time = 0;
				}

				if (!motion)
					done = 1;
			}

			if (motion)
				motion2 = region_shape.FirstMotion(&rnsState);
			else
				motion2 = 0;
			while (motion2 && motion2->Start() != motion->End())
				motion2 = region_shape.NextMotion(&rnsState);
			if (motion2)
			{
				done = 0;
				nulls = 0;
				while (!done)
				{
					new_shape->AddMotion(motion2->NewCopy());
					region_shape.CurrentUntransMotion(&rnsState)->IsCommon(TRUE);
					done = motion2->End() == new_shape->StartPoint();
					motion2 = region_shape.NextMotion(&rnsState);
					if (!motion2)
					{
						nulls ++;
						motion2 = region_shape.FirstMotion(&rnsState);
					}
					if (nulls > 2)
					{
	#ifdef DEVELOPMENT2
						MessageBox(NULL, _T("Cannot create void!"), _T("Voids  1"), MB_APPLMODAL);
	#endif
						done = 1;
					}
				}
				_void_group->Add(new_shape);
            new_shape->Initialize(FALSE, MNS_INIT_NOTHING);
				region_shape.DeleteCommonMotions(&rnsState);
				shape.DeleteCommonMotions(&rnsState);
				new_shape = 0;
   			motion = shape.FirstMotion(&nsState);
			}
         else
         {
	#ifdef DEVELOPMENT2
						MessageBox(NULL, _T("Cannot create void!"), _T("Voids  2"), MB_APPLMODAL);
	#endif
            if (new_shape)
            {
               delete(new_shape);
               new_shape = 0;
               shape.UnmarkCommonMotions();
               region_shape.UnmarkCommonMotions();
				shape.CurrentUntransMotion(untrans_motion, &nsState);
            }
            motion = shape.NextMotion(&nsState);
			}
		}
		else
			motion = shape.NextMotion(&nsState);
	}
}

void MtcPartAnalyzer::AnalyzeRighteousness()
{
	// Should record righteous angles...
	if (!Cluster() || !ClusterScore())
		return;
	else
	{
		MtcNestShape
			*nshape;
		MtcNestMotion
			*prev,
			*next;
      int
         stop(0);
		MtcNestShapeGroupState
			nsgState;
		MtcNestShapeState
			nsState;

		// distance_tolerance is 1/4 the average of the region height + width
		double
			angle,
			distance_tolerance(((ClusterScore()->MinNonLeadsRegion().Height() + ClusterScore()->MinNonLeadsRegion().Width()) / 2.0) / 4.0);
      int
         found;
		nshape = this->working_nsg.FirstNestShape(&nsgState);
		while (nshape)
		{
			found = nshape->FirstAngle(&angle, &nsState, &prev, &next);
			while (found)
			{
				if (fabs(angle - 90.0) < Weights()->RightAngleThreshold()
               && prev->IsLine() && next->IsLine()
               && prev->End() == next->Start()
					&& ClusterScore()->EdgyLine(prev->Start(), prev->End()) && ClusterScore()->EdgyLine(next->Start(), next->End()))
					ClusterScore()->AddRighteousPair(prev->NewCopy(), next->NewCopy(), angle);
				found = nshape->NextAngle(&angle, &nsState, &prev, &next);
			}

			//
			// Look for non-contiguous righteous pairs (rounded corners, notched corners, etc.)
			//		For each "prev" line, only the first suitable "next" line that forms a righteous pair will be saved
			//
			prev = nshape->FirstUntransMotion(&nsState);
			while(prev)
			{
				// First, find an edgy line
				if (prev->IsLine() && ClusterScore()->EdgyLine(prev->Start(), prev->End()))
				{
					// Skip over the next entity (if it were righteous, it'd already be in the list)
					// Reset to the first entity if you fall off the end of the list
					next = nshape->NextUntransMotion(&nsState);
					if (!next)
						next = nshape->FirstUntransMotion(&nsState);
					next = nshape->NextUntransMotion(&nsState);
					if (!next)
						next = nshape->FirstUntransMotion(&nsState);
					// Keep going until we get back to the "prev" entity or we find a righteous situation
					stop = 0;
					while (next && next != prev && !stop)
					{
						// Need an edgy line that starts within a certain distance of the end of "prev"
						if (next->IsLine() && ClusterScore()->EdgyLine(next->Start(), next->End())
							&& prev->End().Distance(next->Start()) < distance_tolerance)
						{
							// If angle between prev and next is about 90 degrees, we've got another righteous pair
							angle = dynamic_cast<MtcNestLine *>(prev)->AngleInDegrees() - dynamic_cast<MtcNestLine *>(next)->AngleInDegrees();
							if (angle > 360.0 - ACCY)
								angle -= 360.0;
							else if (angle < -ACCY)
								angle += 360.0;
							if (fabs(angle - 90.0) < Weights()->RightAngleThreshold())
							{
								ClusterScore()->AddRighteousPair(prev->NewCopy(), next->NewCopy(), angle);
								stop = 1;
							}
						}
						next = nshape->NextUntransMotion(&nsState);
						if (!next)
							next = nshape->FirstUntransMotion(&nsState);
					}
				}
				nshape->CurrentUntransMotion(prev, &nsState);
				prev = nshape->NextUntransMotion(&nsState);
			}
			nshape = this->working_nsg.NextNestShape(&nsgState);
		}
		ClusterScore()->RighteousnessScore((int)(100.0 * ClusterScore()->NumberOfRighteousPairs() / (working_nsg.NumberOfItems() * 4.0)));
	}
}

void MtcPartAnalyzer::AnalyzeSlopiness()
{
	// Should build slopiness pairs...
	if (!Cluster() || !ClusterScore())
		return;
	else
	{
		// Slopey pairs must possess the following attributes:
		//		0. Must have 2 lines
		//		1. Lines must not be in concavities
		//		2. Lines must overlap by some percentage
		//		3. Angle between the lines must be below some tolerance
		MtcNestShape
			*nshape;
		MtcNestMotion
			*motion1,
			*motion2;
		MtcNestLine
			line1,
			line2;
		double
			angle1,
			angle2;
		POINT_2D
			start,
			end;
		int
			overlap_score,
			length_ratio,
			angle_score,
			slopiness(ClusterScore()->SlopinessScore());
		MtcNestShapeGroupState
			nsgState;
		MtcNestShapeState
			nsState;
		nshape = this->working_nsg.FirstNestShape(&nsgState);
		while (nshape)
		{
			motion1 = nshape->FirstUntransMotion(&nsState);
			while (motion1)
			{
				if (motion1->IsLine() && !motion1->InConcavity())
				{
					motion2 = nshape->NextUntransMotion(&nsState);
					while (motion2)
					{
						if (motion2->IsLine() && !motion2->InConcavity())
						{
							line1 = *(dynamic_cast<MtcNestLine *>(motion1));
							line2 = *(dynamic_cast<MtcNestLine *>(motion2));
							angle1 = line1.AngleInRadians();

							//
							// The following code is ugly!  Fix it!!!
							//

							//
							// Check line2 in relation to line1
							//
							start = line2.Start();
							end = line2.End();
							rotate(start, line1.Start(), -angle1);
							rotate(end, line1.Start(), -angle1);
							line2.Set(start, end);
							end = line1.End();
							rotate(end, line1.Start(), -angle1);
							line1.Set(line1.Start(), end);

							angle1 = line1.AngleInDegrees();   // Should be 0.0 now
							angle2 = line2.AngleInDegrees();

							if ((angle2 > 180.0 + ACCY && angle2 < (180.0 + SLOPINESS_THRESHOLD)) || (angle2 < 180.0 - ACCY && (angle2 > 180.0 - SLOPINESS_THRESHOLD)))  // These angle limits are bogus.  Refine them!
							{
								// lines are slopey, but do they overlap at all?
								if ((line2.Start().X() > line1.Start().X()) && (line2.End().X() < line1.End().X()))
								{	
									// overlap score tells how close the center of the lines are to each other - 100% means the center points are one above the other
									overlap_score = (int)(100 * min(line2.Start().X() - line1.Start().X(), line1.End().X() - line2.End().X())/
										max(line2.Start().X() - line1.Start().X(), line1.End().X() - line2.End().X()));
									length_ratio = (int)(100 * min(line1.Length(), line2.Length())/max(line1.Length(), line2.Length()));

									angle_score = (int)(100.0 - (fabs(angle2 - 180)/SLOPINESS_THRESHOLD) * 50);
									angle2 = fabs(angle2 - 180);
									ClusterScore()->AddSlopinessPair(motion1->NewCopy(), motion2->NewCopy(), angle2, overlap_score, length_ratio, angle_score);
								// How do we compute a slopiness score?
									// This will use the closest centerpoints to each other of the lines that are closest to the same size
									//	Do we need to work in angle, also????
									slopiness = max(slopiness, ((overlap_score * length_ratio / 100) + angle_score)/2);
								}
							}
							angle1 = line2.AngleInRadians();

							//
							// The following code is ugly!  Fix it!!!
							//

							//
							// Check line1 in relation to line2
							//
							start = line1.Start();
							end = line1.End();
							rotate(start, line2.Start(), -angle1);
							rotate(end, line2.Start(), -angle1);
							line1.Set(start, end);
							end = line2.End();
							rotate(end, line2.Start(), -angle1);
							line2.Set(line2.Start(), end);

							angle1 = line2.AngleInDegrees();   // Should be 0.0 now
							angle2 = line1.AngleInDegrees();

							if ((angle2 > 180.0 + ACCY && angle2 < (180.0 + SLOPINESS_THRESHOLD)) || (angle2 < 180.0 - ACCY && (angle2 > 180.0 - SLOPINESS_THRESHOLD)))  // These angle limits are bogus.  Refine them!
							{
								// lines are slopey, but do they overlap at all?
								if ((line1.Start().X() > line2.Start().X()) && (line1.End().X() < line2.End().X()))
								{	
									// overlap score tells how close the center of the lines are to each other - 100% means the center points are one above the other
									overlap_score = (int)(100 * min(line1.Start().X() - line2.Start().X(), line2.End().X() - line1.End().X())/
										max(line1.Start().X() - line2.Start().X(), line2.End().X() - line1.End().X()));
									length_ratio = (int)(100 * min(line1.Length(), line2.Length())/max(line1.Length(), line2.Length()));

									angle_score = (int)(100.0 - (fabs(angle2 - 180)/SLOPINESS_THRESHOLD) * 50);
									angle2 = fabs(angle2 - 180);
									ClusterScore()->AddSlopinessPair(motion2->NewCopy(), motion1->NewCopy(), angle2, overlap_score, length_ratio, angle_score);
								// How do we compute a slopiness score?
									// This will use the closest centerpoints to each other of the lines that are closest to the same size
									//	Do we need to work in angle, also????
									slopiness = max(slopiness, ((overlap_score * length_ratio / 100) + angle_score)/2);
								}
							}
						}
						motion2 = nshape->NextUntransMotion(&nsState);
					}
					nshape->CurrentUntransMotion(motion1, &nsState);
				}
				motion1 = nshape->NextUntransMotion(&nsState);
			}
			nshape = this->working_nsg.NextNestShape(&nsgState);
		}
		ClusterScore()->SlopinessScore(slopiness);
	}
}

void MtcPartAnalyzer::AnalyzeEdginess()
{
	// Should record edginess angles...
	if (!Cluster() || !ClusterScore())
		return;
	else
	{
		// Create a nest shape group for the parts in the cluster (but only keep the profile's nest shapes) (use break part shapes if the part has break parts)
		// Remove concavities and slots
		// Check for common lines
		// Rotate the common lines to 0 degrees (and the cluster with them)
		// Check if the common line is in common with the bottom of the cluster's region - if so, keep it and score it
		MtcNestShape
			*nshape1,
			nshape2,
			slotless,
			*concavity,
			*nshape;
		MtcNestMotion
			*motion,
			*motion2,
         *prev,
         *next,
         *prev_copy,
         *next_copy;
		MtcNestLine
			*line,
			*save_line;
		double
			score(0.0),
			max_dim_score,
			region_score,
			area,
			n_area,
			perimeter_threshold,
			max_dim,
         min_dim,
			angle,
			y;
		POINT_2D
			center,
         start,
         end;
		MTC_BOOLEAN
			keep_line(TRUE);
		MtcNestShapeGroupState
			nsgState;
		MtcNestShapeState
			nsState,
			cnsState,
			slnsState;

		BuildNestShapeGroup(&(this->original_nshapes));
      this->working_nsg.Clear();
		nshape = this->original_nshapes.FirstNestShape(&nsgState);
		while (nshape)
		{
			// Check each concavity for size
			//	- if concavities exceed a certain size, the lines created by replacing them should not be considered
			max_dim = ClusterScore()->MinNonLeadsRegion().get_max_dimension();
			min_dim = min(ClusterScore()->MinNonLeadsRegion().Height(), ClusterScore()->MinNonLeadsRegion().Width());
			perimeter_threshold = max_dim * Weights()->EdginessPerimeterFactor();
			nshape1 = (MtcNestShape *)nshape->NewCopy();
			motion = nshape1->FirstUntransMotion(&nsState);
			while (motion)
			{	
				if (motion->IsArc() && !motion->InConcavity())
				{
					MtcNestArc
						*arc = dynamic_cast<MtcNestArc *>(nshape1->CurrentUntransMotion(&nsState));
					if (arc->Sagitta() < arc->Radius() * Weights()->SagittaRatio())  // included angle in the arc of < 4.25 degrees, or so
					{
						// Replace the arc with a line
						MtcNestLine
							*line;
						line = new MtcNestLine(arc->Start(), arc->End(), 0, 0, 0, arc->Quality());
						nshape1->InsertMotion(line, motion);
						nshape1->DeleteMotion(motion, &nsState);
						nshape1->CurrentUntransMotion(line, &nsState);
					}
					else if (fabs(arc->IncludedAngleInDegrees() - 90.0) < Weights()->RightAngleThreshold()
						&& (pow(arc->Radius(), 2.0) * 0.785) < nshape->Area() * Weights()->EdgyRadiusFactor()
						&& arc->Radius() < 0.25 * min_dim)
					{
						// check size and remove arc if small enough... and if adjacent motions are both lines...  that are nearly at right angles to each other...
						// but how small is "small enough?"
						next = nshape1->NextUntransMotion(&nsState);
						if (!next)
							next = nshape1->FirstUntransMotion(&nsState);
						nshape1->CurrentUntransMotion(motion, &nsState);
						prev = nshape1->PrevUntransMotion(&nsState);
						if (!prev)
							prev = nshape1->LastUntransMotion(&nsState);
						nshape1->CurrentUntransMotion(motion, &nsState);

						if (prev && next && prev != next && prev != motion && next != motion && prev->IsLine() && next->IsLine())
						{
							prev_copy = prev->NewCopy();
							next_copy = next->NewCopy();
							prev_copy->ExtendBy(0.0, arc->Radius());
								// This should be the same as _T("next_copy->Extend(arc->Radius(), 0.0)"), but allows for the angle to be not exactly 90 degrees.
							next_copy->Set(prev_copy->End(), next_copy->End());
							if (fabs(prev_copy->AngleWith(next_copy) - 90.0) < Weights()->RightAngleThreshold())
							{
								prev->ExtendBy(0.0, arc->Radius());
								next->Set(prev->End(), next->End());
								nshape1->DeleteMotion(motion, &nsState);
								if (prev != nshape1->LastMotion(&nsState))
									motion = nshape1->CurrentUntransMotion(prev, &nsState);
								else
									motion = nshape1->CurrentUntransMotion(next, &nsState);

							}
						}
					}
				}
				motion = nshape1->NextUntransMotion(&nsState);
			}
			slotless = *nshape1;
			nshape1->RemoveSlots(1.0);
			slotless.RemoveSlots(1.0);
			nshape1->RemoveConcavities();
			concavity = slotless.FirstConcavity(&slnsState);
			n_area = nshape1->Area();
			while (concavity)
			{
				area = concavity->Area();
				keep_line = (area < n_area * 0.10);
				if (!keep_line)
				{
					concavity->MarkCommonMotions(nshape1);
					nshape1->UnmarkCommonMotions();
					nshape1->Smooth(&nsState);
					motion = concavity->FirstCommonMotion(&cnsState);
					while (motion)
					{
						if (nshape1->Within(motion->Start(), 0.001, &motion2))
                  {
                     motion = concavity->NextCommonMotion(&cnsState);
                     motion = concavity->CurrentUntransMotion(&cnsState);
							nshape1->DeleteMotion(motion2, &nsState);
                     concavity->CurrentUntransMotion(motion, &cnsState);
                     motion = concavity->CurrentMotion(&cnsState);
                  }
                  else
						motion = concavity->NextCommonMotion(&cnsState);
					}
				}
				concavity = slotless.NextConcavity(&slnsState);
			}
			this->working_nsg.Add(nshape1);
         nshape = this->original_nshapes.NextNestShape(&nsgState);
		}

		nshape = this->working_nsg.FirstNestShape(&nsgState);
		while (nshape)
		{
			if (nshape->TransDirectionOp() == MCCLW)
				nshape->Reverse(&nsState);
			// Check each line segment after removing concavities to see how long it is
			line = nshape->LongestLine(&nsState);
			while (line && line->Length() > perimeter_threshold)
			{
				// Record the angle of the line
            if (nshape->TransDirectionOp() == MCLW)
				angle = 180.0 - line->AngleInDegrees();
            else
               angle = 360.0 - line->AngleInDegrees();
				if (angle < -ACCY)
					angle += 360.0;
				if (angle < ACCY)
					angle = 0.0;
				center = working_nsg.Center();
				start = line->Start();
				end = line->End();
				save_line = nshape->CurrentUntransLine(&nsState);
				working_nsg.Rotate(angle, center);
				line = nshape->SetAndTransformCurrentLine(save_line, &nsState);
				y = working_nsg.Region().YMin();
				// If line exists on bottom of region of rotated cluster, line is on outside of cluster and therefore "edgy"
				if (fabs(line->Start().Y() - y) < ACCY && fabs(line->End().Y() - y) < ACCY)
				{
					max_dim_score = (line->Length() / max_dim) * 100.0;
					region_score = (line->Length() / nshape->Region().Width()) * 100.0;
					score = max(score, region_score);
					ClusterScore()->AddEdginessValue(angle, max_dim_score, region_score, line->Length(), start, end);
				}
				working_nsg.Rotate(-angle, center);
				nshape->SetAndTransformCurrentLine(save_line, &nsState);
				line = nshape->NextLongestLine(&nsState);
			}
			nshape = this->working_nsg.NextNestShape(&nsgState);
		}
		// What about the edginess score?  Should it be the number of edgy lines?  Should it be related
		//		to the percentage of the profile which is edgy?  Should the concavities be worked in somehow?
		//		The average of the edginess of the edgy lines?  The highest edgy line score?
		ClusterScore()->EdginessScore((int)score);

		// Also, it would be nice to know if a concavity opened onto the side away from the edgy line - 
		//		maybe that line could then get an "edginess bonus".  Or maybe concavularitiness will handle this?
		// Create voids in the current nest shape, check to see how many of the voids have lines in common with the top of the
		//		nest shape's region.  Add up this amount and divide by the total void area???
	}
}

void MtcPartAnalyzer::AnalyzeFillerRotation()
{
	// Try going one step further than what we tried for pattern array;
	//		i.e. if trying 90 for pattern array, try 45 for filling
	// This may be too simplistic, but at least it's a start
	if (!Cluster() || !ClusterScore())
		return;
	else
	{
		int
			angle(90);
			
		switch (ClusterScore()->PatternArrayRotation())
		{
			case 90:
				angle = 45;
				break;
			case 45:
				angle = 30;
				break;
			case 30:
				angle = 15;
				break;
			case 15:
				angle = 10;
				break;
			case 10:
				angle = 5;
				break;
			case 5:
			default:
				angle = 1;
				break;
		}

		if (angle > 1 && this->ClusterScore()->Cluster()->MinRegion().get_max_dimension() /
			Region().get_max_dimension() > 0.99)
			angle = 1;

		ClusterScore()->FillerRotation(angle);
	}
}

void MtcPartAnalyzer::AnalyzeFilleriness()
{
	// This is pretty basic at this point - if the cluster might fit in the region and has a quantity of at least 1,
	//		try it as a filler part
	// In the future, we may want to work in the SpaceMap somehow, rather than looking at the raw region
	if (!Cluster() || !ClusterScore())
		return;
	else if (Region().is_empty() || ClusterScore()->EstimatedFootprintQuantity() == 0 || 
		Cluster()->get_quantity(defaults.get_nest_parameters().IncludeSimilarParts()) == 0)
		ClusterScore()->FillerPartScore(0);
	else
	{
		int
			score(1);

		score = (int)((1 - ClusterScore()->ProfileRatio()) * 60 + 40 * (Cluster()->get_priority() == 99));
		ClusterScore()->FillerPartScore(score);
	}
}

void MtcPartAnalyzer::AnalyzeProfileNestingScore()
{
	if (!Cluster() || !ClusterScore())
		return;
		// Autonesting score would be something like:
	// RETHINK THIS!!! IT ISN'T QUITE RIGHT!!!
		//	(ProfileRatio()/RegionRatio() * 50) + (EstimatedFootprintQuantity() * ProfileRatio() * (10 * EstimatedFootprintQuantity() up to 10)) /2
	// Small quantities and big parts should be good for autonesting, as should large quantities of small parts

   if (ClusterScore()->EstimatedFootprintQuantity() == 0)
      ClusterScore()->ProfileNestingScore(UNDEFINED_SCORE);
   else
   {
   	double
   		score,
         base_score;
   	if (ClusterScore()->ProfileRatio() - Weights()->LargestPratioThreshold() > -ACCY)
   	{
   		score = Weights()->LargestPratioScore();
   		base_score = Weights()->LargestPratioBaseScore();
   	}
   	else if (ClusterScore()->ProfileRatio() - Weights()->LargePratioThreshold() > -ACCY)
   	{
   		score = Weights()->LargePratioScore();
   		base_score = Weights()->LargePratioBaseScore();
   	}
   	else if (ClusterScore()->ProfileRatio() - Weights()->MediumPratioThreshold() > -ACCY)
   	{
   		score = Weights()->MediumPratioScore();
   		base_score = Weights()->MediumPratioBaseScore();
   	}
   	else if (ClusterScore()->ProfileRatio() - Weights()->SmallPratioThreshold() > -ACCY)
   	{
   		score = Weights()->SmallPratioScore();
   		base_score = Weights()->SmallPratioBaseScore();
   	}
   	else
   	{
   		score = Weights()->SmallestPratioScore();
   		base_score = Weights()->SmallestPratioBaseScore();
   	}

#ifdef OLD_SCORES
   	ClusterScore()->ProfileNestingScore(
   		((ClusterScore()->ProfileRatio() * 100.0 + base_score
   		+ (score * (1 - ClusterScore()->ProfileRatio() * ClusterScore()->EstimatedFootprintQuantity()))) / 2.0) + 0.5);
#else

		//	Alternate nesting score calc; use with alternate set of PRATIO values
		ClusterScore()->ProfileNestingScore((int)(
			(ClusterScore()->ProfileRatio() * 100.0 + base_score - (score * (1 - ClusterScore()->ProfileRatio())))/2.0));
#endif
   }
}

void MtcPartAnalyzer::AnalyzeConcavityOrientation()
{
	// Record orientations in which "large" concavities are placed toward center of plate
	//		Is this dependent on where the part is going to be nested, or can we just assume the angles will be
	//		relative angles which place the concavities away from the bottom edge of the plate?
	if (!Cluster() || !ClusterScore())
		return;
}

MtcClusterScore *MtcPartAnalyzer::ClusterScore()
{
	if (this->cluster_score)
		return (this->cluster_score);
	else if (Cluster())
		return (Cluster()->ClusterScore());
	else
		return (0);
}

void MtcPartAnalyzer::ClusterScore(MtcClusterScore *_cluster_score, int _assign_to_cluster)
{
	this->cluster_score = _cluster_score;
	if (Cluster() && _assign_to_cluster)
		Cluster()->ClusterScore(_cluster_score);
}

void MtcPartAnalyzer::BuildNestShapeGroup(MtcNestShapeGroup *_ngroup, CLUSTER *_cluster)
{
	CLUSTER
		*cluster;
	NEST_PART
		*np,
		*bp;
	META_PART
		*mp;
	MtcProfileShape
		*pshape;
	MtcNestShape
		*nshape;
	MtcClusterState
		clState;
	MtcProfileShapeState
		psState;
	MtcNestShapeGroupState
		nsgState;

	if (_cluster)
		cluster = _cluster;
	else
		cluster = Cluster();
	if (!cluster || !_ngroup)
		return;

	_ngroup->Clear();
	np = cluster->get_first_part(&clState);
	while (np)
	{
		if (np->MultiClc())
		{
			mp = np->GetTransformedMultiClcMetaGeometry(0);
			pshape = mp->ProfileShape(TRUE);
			if (pshape)
			{
				nshape = pshape->FirstNestShape(&psState);
				while (nshape)
				{
					if (nshape->IsProfile() && nshape->IsCut())
						_ngroup->Add(nshape->NewCopy());
					nshape = pshape->NextNestShape(&psState);
				}
			}
			delete (mp);
		}
		else if (np->NumberOfBreakParts() > 0)
		{
			bp = np->FirstBreakPart(clState.PartState());
			while (bp)
			{
				mp = bp->get_trans_metapart();
				pshape = mp->ProfileShape(TRUE);
				if (pshape)
				{
					nshape = pshape->FirstNestShape(&psState);
					while (nshape)
					{
						if (nshape->IsProfile() && nshape->IsCut())
							_ngroup->Add(nshape->NewCopy());
						nshape = pshape->NextNestShape(&psState);
					}
				}
				delete (mp);
				bp = np->NextBreakPart(clState.PartState());
			}
		}
		else
		{
			mp = np->get_trans_metapart();
			pshape = mp->ProfileShape(TRUE);
			if (pshape)
			{
				nshape = pshape->FirstNestShape(&psState);
				while (nshape)
				{
					if (nshape->IsProfile() && nshape->IsCut())
						_ngroup->Add(nshape->NewCopy());
					nshape = pshape->NextNestShape(&psState);
				}
			}
			delete (mp);
		}
		np = cluster->get_next_part(&clState);
	}

	// Make sure all nest shapes are clockwise...
	nshape = _ngroup->FirstNestShape(&nsgState);
	while (nshape)
	{
		if (nshape->TransDirectionOp() == MCCLW)
			nshape->Reverse(nsgState.CurrentNestShapeState());
		nshape = _ngroup->NextNestShape(&nsgState);
	}
}

#if defined(MTC_CLASS_TEST_)

#include "clustgrp.hpp"
#include "btimer.hpp"

MtcPartAnalyzerTest::MtcPartAnalyzerTest(CLUSTER_GROUP *_cgrp, JOB *_job, wostream *_stream)
{
	stream = _stream;
	cgrp = _cgrp;
	job = _job;
}

MtcPartAnalyzerTest::~MtcPartAnalyzerTest()
{
}

void MtcPartAnalyzerTest::Test()
{
	AnalyzeTest();
}

void MtcPartAnalyzerTest::AnalyzeTest()
{
	*stream << _T("\n\nStart of MtcPartAnalyzerTest::AnalyzeTest()...") << endl;

	MtcPartAnalyzer
		analyzer;
	REGION
		region(0.0, 0.0, 55.0, 95.0);
   CLUSTER
      *cluster;
	MtcItemState
		*iState;
	MtcClusterState
		clState;
	TCHAR
		buffer[MAXPATH];

MtcBetterTimer
	timer1,
	timer2,
	timer3;

	*stream << _T("Analyzer with no cluster or region assigned: ") << endl;
	*stream << analyzer;
	*stream << _T("\n\n\nAnalyzer with region but no cluster:") << endl;
	analyzer.Analyze(0, region, region, region.get_area(), job);
	*stream << analyzer;

	iState = cgrp->CreateNewState();
timer3.Start();
timer3.Pause();
for(int q = 0; q < 1; q ++)
{
timer2.Start();
timer2.Pause();
	cluster = cgrp->get_first_cluster(iState);
	while(cluster)
	{
timer3.Resume();
timer2.Resume();
timer1.Start();
		analyzer.Analyze(cluster, region, region, region.get_area(), job, cluster->ClusterScore());
timer1.Stop();
timer2.Pause();
timer3.Pause();
		*stream << _T("\nAnalyzing cluster [") << cluster << _T("]:") << endl;
      *stream << _T("(Part name: ") << cluster->get_first_part(&clState)->get_name(buffer) << endl;
		*stream << analyzer;
		*stream << _T("\t\t\tElapsed Time: ") << timer1.ElapsedTimeSeconds() << endl;
      cluster = cgrp->get_next_cluster(iState);
	}
timer2.Stop();
		*stream << _T("\tTotal Elapsed Time: ") << timer2.ElapsedTimeSeconds() << endl;
	if (q % 2)
		region.Set(region.XMin(), region.YMin(), region.XMax() + 0.107, region.YMax());
	else
		region.Set(region.XMin(), region.YMin(), region.XMax(), region.YMax() + 0.089);
	cluster = cgrp->get_first_cluster(iState);
	while (cluster)
	{
		if (cluster->ClusterScore())
		{
			delete(cluster->ClusterScore());
			cluster->ClusterScore(0);
		}
		cluster = cgrp->get_next_cluster(iState);
	}
}
timer3.Stop();
delete (iState);
		*stream << _T("\tTotal Total Elapsed Time: ") << timer3.ElapsedTimeSeconds() << endl;
	*stream << _T("\nEnd of MtcPartAnalyzerTest::AnalyzeTest()...") << endl << endl;

}
#endif
