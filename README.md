# hyperthermDAPL
Project for Hypertherm for the Thayer School of Engineering Data Analytics Project Lab course. 
Professor Geoffrey Parker 
Spring Term 2023
Team Members: Jeremy Hadfield, Jackeline Corona, Ashaunte Hill  
Project title: Carbon emission reduction through better use of nesting algorithms

## Project background 

### Docs for the project
* [Feature engineering doc](https://docs.google.com/document/d/1gGxdz6Rt-ZfFC753otcyBDhUmH_BHBFCZhgUyQKGBUE/edit?usp=sharing)
* Ask me directly for other docs at jhadfield129@gmail.com or jeremy.th@dartmouth.edu 
### Hypertherm
Hypertherm was founded in 1968 and is headquartered in Hanover, NH. It has around 2,000 employees and is a 100% employee-owned company. The company has a strong global presence with operations and partner representation across several continents. It is a leading manufacturer of industrial cutting systems and software solutions. The company specializes in producing advanced plasma, laser, and waterjet cutting technologies, which are used in a variety of industries, from manufacturing to shipbuilding. Their innovative, diverse product portfolio of advanced tools and software help businesses precisely cut and shape materials for their specific needs, resulting in higher productivity, better product quality, and reduced operating costs. By integrating advanced cutting technologies with intelligent software solutions, Hypertherm helps businesses efficiently cut and shape steel, ultimately saving time, money, and resources.

### Project goals & scope 
The overarching purpose of this project is to reduce the environmental impact of steel sheet cutting through the optimization of nesting algorithms. Hypertherm currently struggles to optimize the utilization of steel sheets during the cutting process. Existing cutting processes produce substantial waste in the form of steel scrap, which is by far the largest environmental impact in Hypertherm’s entire value chain. Further, customer habits in algorithm selection are a barrier, as users often simply apply the default nesting algorithm in the cutting software rather than using the most efficient method. In this project, we plan to use Hypertherm’s CEIP (Customer Experience Improvement Program) dataset to analyze the relationship between part features, nesting features, and nesting algorithms. Ultimately, we aim to use the features in this data to predict which combination of parts and algorithms maximizes utilization. 
## Database Info

* Unique values (number of unique jobs)
* dbo.Part - 2637599 - 2.6 million
* dbo.Nest - 3688620 - 3.7 million
* dbo.AutoNest - 865559 - 865 thousand 

SELECT COUNT(*) FROM dbo.Part => 30,970,284 rows 
SELECT COUNT(*) FROM dbo.Nest => 8,711,803 rows 
SELECT COUNT(*) FROM dbo.AutoNest => 1,911,518 rows 
NUMBER OF DISTINCT JOBS: 
SELECT COUNT(DISTINCT ixJobSummary) AS JobCount FROM dbo.Part => 2,637,599 jobs 
SELECT COUNT(DISTINCT ixJobSummary) AS JobCount FROM dbo.Nest => 3,688,620 jobs 
NUMBER OF JOBS SHARED BETWEEN dbo.NEST & dbo.Part => 2,537,550 
Jobs in dbo.Part but not in dbo.Nest => 49 
Jobs in dbo.Nest but not in dbo.Part => 1,051,070 

## Tasks 

## Feature Engineering 
* The `feature_selection` folder contains the SQL scripts used to select features for the project. Most of them are just trials or attempts and are not used in the final project.
* `feature_selection/feature_select_filter.sql` - SQL script to filter out features that are not useful for the project. 
* **Filtering applied in this script: **
  * Only include jobs where there are at least two nests (fn.NestCount > 1)
  * Include only rectangular plates. n.ixPlateType IN (0, 1) to include circular plates (1)
  * Exclude nests with safe zones (ensure n.cSafeZone = 0)
  * Exclude multi-torch jobs (ensure n.cMaxTorches <= 1)
  * Exclude jobs without any cuts (where cTimesCut = 0)
  * Exclude jobs where dSheetArea is 0 
  * Exclude jobs that are not from the right ProNest software versions 
* Test 

## Results
* Data cleaning 
* Documentation of the database, the task, and the process 
* Explanation of the project and why it's important 
* Verification of LCAs - based on the utilization numbers in the database, analyze how much money, steel, and carbon emissions could be prevented by increasing utilization. Give the environmental context and explain how this would help Hypertherm achieve its sustainability goals. If possible, give specifics - e.g. if utilization was increased by 20% across the board, what would that give Hypertherm? What strategies or changes could increase utilization like this? 
* Proof that ML/DL is a productive approach 
* Our well-documented, well-commented code in a Github, along with our cleaned CSVs 
* Exploratory data analysis 
* Data visualizations 
* Machine learning model initial results 
* Recommendations for what data might be needed in the future 

## Resources & Tools Used 
* [Azure Data Studio](https://learn.microsoft.com/en-us/sql/azure-data-studio/download-azure-data-studio?view=sql-server-ver16&culture=en-us&country=us&tabs=redhat-install%2Credhat-uninstall) - for SQL management
* Github Copilot - AI coding assistant, free with Github For Education - Github For Education](https://education.github.com/) - free for students
* Tableau - for data visualization
* SQL - for database management and queries 
* Dartmouth Databases - research computing dashboard 
* Azure - attempted to use but did not end up using for the database management 
* Bing
* ChatGPT - especially GPT-4 is good 
* Python - pandas, scikit-learn, etc 

## Learning Resources
* Kaggle Learn - https://www.kaggle.com/learn
* Kaggle Learn Intro to Machine Learning - https://www.kaggle.com/learn/intro-to-machine-learning 

## Exploratory data analysis

## Data Hypertherm Might Need in the Future
