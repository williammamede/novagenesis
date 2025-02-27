import { Component, OnInit } from '@angular/core';
import { GraphAppService } from './graph-app.service';
import { Edge, Node, ClusterNode } from '@swimlane/ngx-graph';


@Component({
  selector: 'app-graph-app',
  templateUrl: './graph-app.component.html',
  providers: [GraphAppService],
  styleUrls: ['./graph-app.component.css']
})
export class GraphAppComponent implements OnInit {
  constructor(private graphAppService: GraphAppService) { }

  public graphLoaded: boolean = false;
  public data: String = '';
  public links: Edge[] = [];
  // [
  //   {
  //     id: 'a',
  //     source: 'first',
  //     target: 'second',
  //     label: 'is parent of'
  //   },
  //   {
  //     id: 'b',
  //     source: 'first',
  //     target: 'third',
  //     label: 'custom label'
  //   }
  // ];
  public nodes: Node[] = [];
  //   {
  //     id: 'first',
  //     label: 'A'
  //   },
  //   {
  //     id: 'second',
  //     label: 'B'
  //   },
  //   {
  //     id: 'third',
  //     label: 'C'
  //   }
  // ];

  public clusters: ClusterNode[] = [];

  ngOnInit(): void {

    this.graphAppService.getData().subscribe(data => {
      this.data = JSON.stringify(data);
      this.buildEdgesFromData(data);
      this.buildNodesFromData(data);
      this.buildClustersFromData(data);
      this.graphLoaded = true;
    });
  }

  getNgBinding() {
    this.graphAppService.showData();
  }

  buildEdgesFromData(data: any) {
    this.links = [];

    // Iterate over each host in the domain
    data.Domain.Hosts.forEach((host: any) => {
      // Create a link between the domain and the host
      this.links.push({ source: data.Domain.ID, target: host.Identifier, label: 'contains' });

      // Iterate over each OSID in the host
      host.OSIDs.forEach((os: any) => {
        // Create a link between the host and the OSID
        this.links.push({ source: host.Identifier, target: os.OSID, label: 'has' });

        // Iterate over each PID in the OSID
        os.PIDs.forEach((pid: any) => {
          // Create a link between the OSID and the PID
          this.links.push({ source: os.OSID, target: pid.PID, label: 'has' });
          pid.TransferredFiles.forEach((file: any) => {
            // Create a link between the PID and the file
            this.links.push({ source: pid.PID, target: pid.PID + file, label: 'transferred'});
          });
        });
      });
    });
  }

  buildNodesFromData(data: any) {
    this.nodes = [];

    // Add the domain as a node
    this.nodes.push({ id: data.Domain.ID, label: "Domain: " + data.Domain.ID });

    // Iterate over each host in the domain
    data.Domain.Hosts.forEach((host: any) => {
      // Add the host as a node
      this.nodes.push({ id: host.Identifier, label: "Host: " + host.Hostname, data: { showLabel: false } });

      // Iterate over each OSID in the host
      host.OSIDs.forEach((os: any) => {
        // Add the OSID as a node
        this.nodes.push({ id: os.OSID, label: "OS: " + os.OSNAME, data: { showLabel: false } });

        // Iterate over each PID in the OSID
        os.PIDs.forEach((pid: any) => {
          // Add the PID as a node
          this.nodes.push({ id: pid.PID, label: "PID: " + pid.PID, data: { showLabel: false } });
          pid.TransferredFiles.forEach((file: any) => {
            // Add the file as a node
            this.nodes.push({ id: pid.PID + file, label: "File: " + file, data: { showLabel: false } });
          });
        });
      });
    });

  }

  buildClustersFromData(data: any) {
    this.clusters = [
      { id: 'clusterDomain', label: 'Domain', childNodeIds: [] },
      { id: 'clusterHost', label: 'Host', childNodeIds: [] },
      { id: 'clusterOS', label: 'OS', childNodeIds: [] },
      { id: 'clusterPID', label: 'PID', childNodeIds: [] },
      { id: 'clusterFile', label: 'TransferredFile', childNodeIds: [] }
    ];

    // Add the domain to the domain cluster
    (this.clusters.find(cluster => cluster.label === 'Domain')?.childNodeIds ?? []).push(data.Domain.ID);

    // Iterate over each host in the domain
    data.Domain.Hosts.forEach((host: any) => {
      // Add the host to the host cluster
      (this.clusters.find(cluster => cluster.label === 'Host')?.childNodeIds ?? []).push(host.Identifier);

      // Iterate over each OSID in the host
      host.OSIDs.forEach((os: any) => {
        // Add the OSID to the OS cluster
        (this.clusters.find(cluster => cluster.label === 'OS')?.childNodeIds ?? []).push(os.OSID);

        // Iterate over each PID in the OSID
        os.PIDs.forEach((pid: any) => {
          // Add the PID to the PID cluster
          (this.clusters.find(cluster => cluster.label === 'PID')?.childNodeIds ?? []).push(pid.PID);

          // Iterate over each TransferredFile in the PID
          pid.TransferredFiles.forEach((file: any) => {
            // Add the file to the file cluster
            (this.clusters.find(cluster => cluster.label === 'TransferredFile')?.childNodeIds ?? []).push(pid.PID + file);
          });
        });
      });
    });
  }

}
