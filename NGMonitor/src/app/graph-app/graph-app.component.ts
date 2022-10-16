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
  public links: Edge[] = [
    {
      id: 'a',
      source: 'first',
      target: 'second',
      label: 'is parent of'
    },
    {
      id: 'b',
      source: 'first',
      target: 'third',
      label: 'custom label'
    },
    {
      id: 'c',
      source: 'first',
      target: 'fourth',
      label: 'custom label'
    }
  ];
  public nodes: Node[] = [
    {
      id: 'first',
      label: 'A'
    },
    {
      id: 'second',
      label: 'B'
    },
    {
      id: 'third',
      label: 'C'
    },
    {
      id: 'fourth',
      label: 'D'
    }
  ];

  ngOnInit(): void {
    // get binding each 5 seconds
   /*  setInterval(() => {
      this.graphAppService.getData().subscribe(data => {
        this.data = JSON.stringify(data);
    });;
    }, 5000); */
  }

  getNgBinding() {
    this.graphAppService.showData();
  }

}
