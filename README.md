# hyperthermDAPL
Project for Hypertherm for the Thayer School of Engineering Data Analytics Project Lab course. 
Professor Geoffrey Parker 
Spring Term 2023
Team Members: Jeremy Hadfield, Jackeline Corona, Ashaunte Hill  
Project title: Carbon emission reduction through better use of nesting algorithms

## Project background 

### Docs for the project
* [Feature engineering doc](https://docs.google.com/document/d/1gGxdz6Rt-ZfFC753otcyBDhUmH_BHBFCZhgUyQKGBUE/edit?usp=sharing)
* [Final Presentation](https://docs.google.com/presentation/d/1dStRV1ZOExTIJEzfsv0eXDAbw8WIKH-mX-2jgdKO9Rw/edit?usp=drive_link) 
* Ask me directly for other docs at jhadfield129@gmail.com or jeremy.th@dartmouth.edu

### Sample slides that convey the project 
See some of the most important sample slides from our presentation
<img width="1381" alt="image" src="https://github.com/jerhadf/hyperthermDAPL/assets/16784270/30984535-af02-48b7-a2d7-5ad73a142dcc">

<img width="1306" alt="image" src="https://github.com/jerhadf/hyperthermDAPL/assets/16784270/55416675-7800-4f58-9e61-bdb6b63f1d10">

<img width="1363" alt="image" src="https://github.com/jerhadf/hyperthermDAPL/assets/16784270/c267b30a-b8ed-42e4-9812-5c1c022f4e97">

<img width="1378" alt="image" src="https://github.com/jerhadf/hyperthermDAPL/assets/16784270/153df350-a9f2-48ca-8f8d-00046f24846f">

<img width="1393" alt="image" src="https://github.com/jerhadf/hyperthermDAPL/assets/16784270/9e8315f7-0c13-4987-8976-84c5de679b98">

<img width="1315" alt="image" src="https://github.com/jerhadf/hyperthermDAPL/assets/16784270/3b7dbf30-c154-4877-bd45-ec8ec9eab94f">

<img width="1406" alt="image" src="https://github.com/jerhadf/hyperthermDAPL/assets/16784270/66e83355-43bd-46ae-bc3c-82b1c5807858">

<img width="1413" alt="image" src="https://github.com/jerhadf/hyperthermDAPL/assets/16784270/65e4f682-7c0a-46d1-9368-dcced23c5655">

<img width="1151" alt="image" src="https://github.com/jerhadf/hyperthermDAPL/assets/16784270/8d9e0580-aa54-4f70-b212-10e0604bc863">

<img width="1144" alt="image" src="https://github.com/jerhadf/hyperthermDAPL/assets/16784270/eeb99506-d8e2-4c09-9161-e20b50d4aeab">

<img width="1077" alt="image" src="https://github.com/jerhadf/hyperthermDAPL/assets/16784270/6bd17898-9cf9-4883-a916-d49901897d03">

<img width="1364" alt="image" src="https://github.com/jerhadf/hyperthermDAPL/assets/16784270/546dbdca-da87-4ccd-ae21-37433f412c1e">

<img width="684" alt="image" src="https://github.com/jerhadf/hyperthermDAPL/assets/16784270/1f8b5bcd-38d9-427f-adc9-7d3e311ce80b">

<img width="1395" alt="image" src="https://github.com/jerhadf/hyperthermDAPL/assets/16784270/bd1d4da2-0c09-44fc-9075-385d4c1cd82a">

<img width="1172" alt="image" src="https://github.com/jerhadf/hyperthermDAPL/assets/16784270/57397526-c327-4ae9-98d5-3ea66094a66e">

<img width="1140" alt="image" src="https://github.com/jerhadf/hyperthermDAPL/assets/16784270/a511a9ec-6d45-44ad-8f95-3a852b45306b">

<img width="1105" alt="image" src="https://github.com/jerhadf/hyperthermDAPL/assets/16784270/1fa36c21-e14f-4026-aff8-375990bd7b57">

* 🔑 **Key impacts:** This presentation, the documentation, the pipeline, and the ML models we created were later used by the Hypertherm team to start a project to optimize their nesting algorithms using the approach we recommended. If implemented, this project could save hundreds of millions of dollars for their customers, hundreds of thousands of tons of steel, and prevent hundreds of megatons of CO2eq emissions. 


### Hypertherm
Hypertherm was founded in 1968 and is headquartered in Hanover, NH. It has around 2,000 employees and is a 100% employee-owned company. The company has a strong global presence with operations and partner representation across several continents. It is a leading manufacturer of industrial cutting systems and software solutions. The company specializes in producing advanced plasma, laser, and waterjet cutting technologies, which are used in a variety of industries, from manufacturing to shipbuilding. Their innovative, diverse product portfolio of advanced tools and software help businesses precisely cut and shape materials for their specific needs, resulting in higher productivity, better product quality, and reduced operating costs. By integrating advanced cutting technologies with intelligent software solutions, Hypertherm helps businesses efficiently cut and shape steel, ultimately saving time, money, and resources.

### Project goals & scope 
The overarching purpose of this project is to reduce the environmental impact of steel sheet cutting through the optimization of nesting algorithms. Hypertherm currently struggles to optimize the utilization of steel sheets during the cutting process. Existing cutting processes produce substantial waste in the form of steel scrap, which is by far the largest environmental impact in Hypertherm’s entire value chain. Further, customer habits in algorithm selection are a barrier, as users often simply apply the default nesting algorithm in the cutting software rather than using the most efficient method. In this project, we plan to use Hypertherm’s CEIP (Customer Experience Improvement Program) dataset to analyze the relationship between part features, nesting features, and nesting algorithms. Ultimately, we aim to use the features in this data to predict which combination of parts and algorithms maximizes utilization. 

## Database Info

* Unique values (number of unique jobs)
* dbo.Part - 2637599 - 2.6 million
* dbo.Nest - 3688620 - 3.7 million
* dbo.AutoNest - 865559 - 865 thousand 

### Details on selection of data from the SQL database

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
* Impact articulation and ensuring that this project is applied within the organization for emissions reduction & steel saving 

## Resources & Tools Used 
* [Azure Data Studio](https://learn.microsoft.com/en-us/sql/azure-data-studio/download-azure-data-studio?view=sql-server-ver16&culture=en-us&country=us&tabs=redhat-install%2Credhat-uninstall) - for SQL management
* Github Copilot - AI coding assistant, free with Github For Education - Github For Education](https://education.github.com/)
* Tableau - for data visualization
* SQL - for database management and queries 
* Dartmouth Databases - research computing dashboard 
* Azure - attempted to use but did not end up using for the database management 
* Bing
* ChatGPT - especially GPT-4 is good 
* Python - pandas, scikit-learn, etc 
* Kaggle Learn - https://www.kaggle.com/learn - tool for learning and refining ML skills
* Kaggle Learn Intro to Machine Learning - https://www.kaggle.com/learn/intro-to-machine-learning 

## Exploratory data analysis

## Data Hypertherm Might Need in the Future
