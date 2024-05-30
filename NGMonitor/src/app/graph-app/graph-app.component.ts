import { Component, OnInit } from '@angular/core';
import { GraphAppService } from './graph-app.service';
import { Edge, Node } from '@swimlane/ngx-graph';


@Component({
  selector: 'app-graph-app',
  templateUrl: './graph-app.component.html',
  providers: [GraphAppService],
  styleUrls: ['./graph-app.component.css']
})
export class GraphAppComponent implements OnInit {
  constructor(private graphAppService: GraphAppService) { }

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

  ngOnInit(): void {
    // get binding each 5 seconds
    setInterval(() => {
      this.graphAppService.getData().subscribe(data => {
        this.data = JSON.stringify(data);
        this.buildEdgesFromData(data);
        this.buildNodesFromData(data);
      });;
    }, 5000);
  }

  getNgBinding() {
    this.graphAppService.showData();
  }

  buildEdgesFromData(data: any) {
    // BUild edges from data following the pattern bellow
    //{"EAB15025": ["C3C6B84D","J3C6B84D"]
    // that will be {id: random, source: "EAB15025", target: "C3C6B84D", label: "is parent of"}
    // and {id: random, source: "EAB15025", target: "J3C6B84D", label: "is parent of"}
    // and so on
    this.links = [];
    for (let key in data) {
      let value = data[key];
      for (let i = 0; i < value.length; i++) {
        this.links.push({
          id: key + i,
          source: key,
          target: value[i],
          label: 'is parent of'
        });
      }
    }
  }

  buildNodesFromData(data: any) {
    // Build nodes from data following the pattern bellow
    //{"EAB15025": ["C3C6B84D","J3C6B84D"]
    // that will be {id: "EAB15025", label: "EAB15025"}
    // and {id: "C3C6B84D", label: "C3C6B84D"}
    // and {id: "J3C6B84D", label: "J3C6B84D"}
    // and so on
    this.nodes = [];
    for (let key in data) {
      // only push if it is not already in the list
      if (!this.nodes.some(node => node.id === key)) {
        this.nodes.push({
          id: key,
          label: "Source " + key
        });
      }
      let value = data[key];
      for (let i = 0; i < value.length; i++) {
        if (!this.nodes.some(node => node.id === value[i])) {
          this.nodes.push({
            id: value[i],
            label: value[i]
          });
        }
      }
    }
  }

}
