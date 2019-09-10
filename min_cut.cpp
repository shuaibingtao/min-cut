#include <iostream>
#include <vector>
#include <pcl/io/pcd_io.h>
#include <pcl/point_types.h>
#include <pcl/visualization/cloud_viewer.h>
#include <pcl/filters/passthrough.h>
#include <pcl/segmentation/min_cut_segmentation.h>
#include <pcl/filters/extract_indices.h>

using namespace pcl;

int main01(int argc, char** argv)
{
	pcl::PointCloud <pcl::PointXYZ>::Ptr cloud(new pcl::PointCloud <pcl::PointXYZ>);
	if (pcl::io::loadPCDFile <pcl::PointXYZ>("out_put_cluster_51.pcd", *cloud) == -1)
	{
		std::cout << "Cloud reading failed." << std::endl;
		return (-1);
	}

	pcl::IndicesPtr indices(new std::vector <int>);
	pcl::PassThrough<pcl::PointXYZ> pass;
	pass.setInputCloud(cloud);
	pass.setFilterFieldName("z");
	pass.setFilterLimits(0.0, 100.0);
	pass.filter(*indices);

	pcl::MinCutSegmentation<pcl::PointXYZ> seg;
	seg.setInputCloud(cloud);
	seg.setIndices(indices);

	pcl::PointCloud<pcl::PointXYZ>::Ptr foreground_points(new pcl::PointCloud<pcl::PointXYZ>());
	pcl::PointXYZ point;
	point.x = 412.76089478;
	point.y = -415.14398193;
	point.z = 22.50600052;
	foreground_points->points.push_back(point);
	seg.setForegroundPoints(foreground_points);

	seg.setSigma(0.25);
	seg.setRadius(7.0);
	seg.setNumberOfNeighbours(30);
	seg.setSourceWeight(0.8);

	std::vector <pcl::PointIndices> clusters;
	seg.extract(clusters);

	std::cout << "Maximum flow is " << seg.getMaxFlow() << std::endl;

	std::vector<int> index1;//提取1,3,4位置处点云
	for (size_t i = 0; i < clusters[0].indices.size(); i++)
	{
		index1.push_back(clusters[0].indices[i]);
	}
	boost::shared_ptr<std::vector<int>> index_ptr1 = boost::make_shared<std::vector<int>>(index1);
	

	pcl::ExtractIndices<pcl::PointXYZ> extract;
	// Extract the inliers
	extract.setInputCloud(cloud);
	extract.setIndices(index_ptr1);

	pcl::PointCloud <pcl::PointXYZ>::Ptr cloud_fore(new pcl::PointCloud <pcl::PointXYZ>);
	extract.setNegative(false);//如果设为true,可以提取指定index之外的点云
	extract.filter(*cloud_fore);
	pcl::PCDWriter writer1;
	writer1.write<pcl::PointXYZ>("cloud_fore.pcd", *cloud_fore,false);

	pcl::PointCloud <pcl::PointXYZ>::Ptr cloud_back(new pcl::PointCloud <pcl::PointXYZ>);
	extract.setNegative(true);//如果设为true,可以提取指定index之外的点云
	extract.filter(*cloud_back); 
	writer1.write<pcl::PointXYZ>("cloud_back.pcd", *cloud_back, false);

	return (0);
}